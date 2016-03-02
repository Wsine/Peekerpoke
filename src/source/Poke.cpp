#include "Poke.h"

Poke::Poke()
	: m_carName("Car2")
	, m_isForceDataSet(false)
	, m_isUseDigestSha256Set(false)
	, m_isLastAsFinalBlockIdSet(false)
	, m_freshnessPeriod(-1)
	, m_timeout(-1)
	, m_isDataSent(false) {	
}

Poke::Poke(std::string carName)
	: m_carName(carName)
	, m_isForceDataSet(false)
	, m_isUseDigestSha256Set(false)
	, m_isLastAsFinalBlockIdSet(false)
	, m_freshnessPeriod(-1)
	, m_timeout(-1)
	, m_isDataSent(false) {	
}

void Poke::usage() {
	printf("\n Usage:\n [-f] [-D] [-i identity] [-F] [-x freshness] [-w timeout] ndn:/name\n" 
           "   Reads payload from stdin and sends it to local NDN forwarder as a single Data packet\n" 
           "   [-f]          - force, send Data without waiting for Interest\n" 
           "   [-D]          - use DigestSha256 signing method instead of SignatureSha256WithRsa\n" 
           "   [-i identity] - set identity to be used for signing\n" 
           "   [-F]          - set FinalBlockId to the last component of Name\n" 
           "   [-x]          - set FreshnessPeriod in time::milliseconds\n" 
           "   [-w timeout]  - set Timeout in time::milliseconds\n" 
           "   [-h]          - print help and exit\n" 
           "   [-V]          - print version and exit\n" 
           "\n");
}

void Poke::setForceData() {
	m_isForceDataSet = true;
}

void Poke::setUseDigestSha256() {
	m_isUseDigestSha256Set = true;
}

void Poke::setIdentityName(char* identityName) {
	m_identityName = make_shared<Name>(identityName);
}

void Poke::setLastAsFinalBlockId() {
	m_isLastAsFinalBlockIdSet = true;
}

void Poke::setFreshnessPeriod(int freshnessPeriod) {
	if (freshnessPeriod < 0)
		usage();
	m_freshnessPeriod = time::milliseconds(freshnessPeriod);
}

void Poke::setTimeout(int timeout) {
	if (timeout < 0)
		usage();
	m_timeout = time::milliseconds(timeout);
}

void Poke::setPrefixName(char* prefixName) {
	m_prefixName = Name(prefixName);
}

time::milliseconds Poke::getDefaultTimeout() {
	return time::seconds(10);
}

bool Poke::isDataSent() const	{
	return m_isDataSent;
}

void Poke::onTimeout(const Interest& interest) {
	printf("Interest waits for response time out.\n\n");
}

void Poke::onData(const Interest& interest, Data& data) {
	;
}

shared_ptr<Data> Poke::createDataPacket(Name GetInterestName, int SearchResult, std::string aimAtPosition) {
	auto dataPacket = make_shared<Data>(GetInterestName);
	std::string timeStamp = time::toIsoString(time::system_clock::now());
	std::string payload;
	
	payload = Util::int2string(SearchResult);
	payload += "/" + aimAtPosition + "/" + timeStamp + "/" + m_carName + "/";
	m_payload = payload;
	dataPacket->setContent(reinterpret_cast<const uint8_t*>(m_payload.c_str()), m_payload.length());
	if (m_freshnessPeriod >= time::milliseconds::zero()) {
		dataPacket->setFreshnessPeriod(m_freshnessPeriod);
	}
	if (m_isLastAsFinalBlockIdSet) {
		if (!m_prefixName.empty()) {
			dataPacket->setFinalBlockId(m_prefixName.get(-1));
		}
		else {
			std::cerr << "Name Provided Has 0 Components" << std::endl;
			exit(1);
		}
	}
	if (m_isUseDigestSha256Set) {
		m_keyChain.sign(*dataPacket, signingWithSha256());
	}
	else {
		if (m_identityName == nullptr) {
			m_keyChain.sign(*dataPacket);
		}
		else {
			m_keyChain.sign(*dataPacket, signingByIdentity(*m_identityName));
		}
	}	 
	return dataPacket;
}

int Poke::GetInformationFromMemory(int AimPosition) {
	int SearchResult = SEARCH_MAP_FAILED;
	try {
		SearchResult = Util::getCar().getMap().getMapAtPosition(AimPosition);
	} catch (std::exception& e) {
		SearchResult = SEARCH_MAP_FAILED;
	}
	return SearchResult;
}

int Poke::GetAimPosition(std::string getName) {
	char PlaceNumber[2];
	PlaceNumber[0] = getName[7];
	PlaceNumber[1] = getName[8];
	if (PlaceNumber[1] == '/') {
		PlaceNumber[1] = PlaceNumber[0];
		PlaceNumber[0] = '0';
	} 
	int AimPosition = (PlaceNumber[0] - '0') * 10 + PlaceNumber[1] - '0';
	return AimPosition;
}

int Poke::GetTypeOfInterest(std::string AimPosition, Interest interest) {
	int result = -1;
	std::string m_filter_broadcast = "/broadcast";
	std::string m_filter_filter = "/filter/";
	std::string m_filter_common = "/common/";
	std::string init_place = "/place/" + AimPosition;
	Name JudgeBroadcast(init_place + m_filter_broadcast);
	Name JudgeFilter(init_place + m_filter_filter);
	Name JudgeCommon(init_place + m_filter_common);
	if (JudgeBroadcast.isPrefixOf(interest.getName()) == true) {
		result = 1;
	}
	if (JudgeFilter.isPrefixOf(interest.getName()) == true) {
		result = 2;
	}
	if (JudgeCommon.isPrefixOf(interest.getName()) == true) {
		result = 3;
	}
	printf("%s\n", init_place.c_str());
	return result;
}

void Poke::BroadcastData(int AimPosition) {
	std::string str = Util::int2string(AimPosition);
	std::string interest_name = "/place/" + str + "/common/" + m_payload;
	usepeek(interest_name);	
}

void Poke::onInterest(const Name& name, const Interest& interest) {
	m_isDataSent = true;		
	Name GetInterestName = interest.getName();
	std::string getName = GetInterestName.toUri();
	int AimPosition = GetAimPosition(getName);
	std::string AimPosStr = Util::int2string(AimPosition);
	int Type = GetTypeOfInterest(AimPosStr,  interest);
	int SearchResult =  GetInformationFromMemory(AimPosition);

	switch(Type) {
		/* The broadcast model interst,  we should respond it with our data. */
		case 1: {
			printf("Receive interest. The interest is broadcast model. We should send the data.\n\n");
			if (SearchResult != 9) {
				shared_ptr<Data> dataPacket = createDataPacket(GetInterestName, SearchResult, AimPosStr);
				m_face.put(*dataPacket);
				BroadcastData(AimPosition);
			}
			else {
				printf("Search information error.\n");
			}
			break;
		}
		/* We get the interest which is filtered. We should judge whether our car has been banned. */
		case 2: {
			/* To search if our car number is in the filter. */
			unsigned int location = getName.find(m_carName, 0);
			if (location != std::string::npos) {
				printf("The information from this car has been received,  we don't need to send it.\n");
			}
			else {
				printf("We are not in the filter,  we should send the data again.\n");
				if (SearchResult != SEARCH_MAP_FAILED) {
					shared_ptr<Data> dataPacket = createDataPacket(GetInterestName, SearchResult, AimPosStr);
					m_face.put(*dataPacket);
					BroadcastData(AimPosition);
				}
				else {
					printf("Search information error.\n");
				}
			}
			break;
		}
		/* This kind of interest contains data information. Modified by LiBoyang on Feb 9th,  2016. */
		case 3: {
			printf("Someone has sent a data packet: %s\n", getName.c_str());
			break;
		}
		default: {
			printf("Type error!\n");
			break;
		}
	}
}

void Poke::onRegisterFailed(const Name& prefix, const std::string& reason) {
	std::cerr << "Prefix Registration Failure." << std::endl;
	std::cerr << "Reason = " << reason << std::endl;
}

void Poke::run() {
	try {
		m_face.setInterestFilter(m_prefixName,
                         bind(&Poke::onInterest, this, _1, _2),
                         RegisterPrefixSuccessCallback(),
                         bind(&Poke::onRegisterFailed, this, _1, _2));
		m_face.processEvents();
	}
	catch (std::exception& e) {
		std::cerr << "ERROR: " << e.what() << "\n" << std::endl;
		exit(1);
	}
}

void Poke::usepeek(std::string m_interest_name) {
	m_face.expressInterest(createInterestPacket(m_interest_name),
                     bind(&Poke::onData, this, _1, _2),
                     bind(&Poke::onTimeout, this, _1));
}

Interest Poke::createInterestPacket(std::string m_name) {
	Name interestName(m_name);
	Interest interestPacket(interestName);
	interestPacket.setMustBeFresh(true);
	interestPacket.setInterestLifetime(time::seconds(4));
	return interestPacket;
}