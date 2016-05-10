/**
 * @file Poke.cpp
 * @brief The implementation of class Poke
 */

#include "Poke.h"

/**
 * @fn Poke::Poke()
 * @brief Default constructor of class Poke
 * @brief set the attributes of the data packet and producer, has been replaced by Poke::Poke(std::string carName)
 */
Poke::Poke()
	: m_carName("Car2")
	, m_isForceDataSet(false)
	, m_isUseDigestSha256Set(false)
	, m_isLastAsFinalBlockIdSet(false)
	, m_freshnessPeriod(-1)
	, m_timeout(-1)
	, m_isDataSent(false) {	
}

/**
 * @fn Poke::Poke(std::string carName)
 * @brief Default constructor of class Poke
 * @brief set the attributes of the data packet and producer, set car name directly.
 * @param carName the unique ID of the car
 */
Poke::Poke(std::string carName)
	: m_carName(carName)
	, m_isForceDataSet(false)
	, m_isUseDigestSha256Set(false)
	, m_isLastAsFinalBlockIdSet(false)
	, m_freshnessPeriod(-1)
	, m_timeout(-1)
	, m_isDataSent(false) {	
}

/**
 * @fn Poke::usage()
 * @brief introduce kinds of attributes.
 */
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

/**
 * @fn Poke::setForceData()
 * @brief ndn method
 * @brief set attribute of data packet, force to send.
 */
void Poke::setForceData() {
	m_isForceDataSet = true;
}

/**
 * @fn Poke::setUseDigestSha256()
 * @brief ndn method
 * @brief set attribute of data packet, whether use digital signment.
 */
void Poke::setUseDigestSha256() {
	m_isUseDigestSha256Set = true;
}

/**
 * @fn Poke::setIdentityName(char* identityName)
 * @brief ndn method
 * @brief set attribute of data packet
 */
void Poke::setIdentityName(char* identityName) {
	m_identityName = make_shared<Name>(identityName);
}

/**
 * @fn Poke::setLastAsFinalBlockId()
 * @brief ndn method
 * @brief set attribute of data packet
 */
void Poke::setLastAsFinalBlockId() {
	m_isLastAsFinalBlockIdSet = true;
}

/**
 * @fn Poke::setFreshnessPeriod(int freshnessPeriod)
 * @brief ndn method
 * @brief set attribute of data packet, data packet's fresh time.
 * @param freshnessPeriod, data packet's fresh time.
 */
void Poke::setFreshnessPeriod(int freshnessPeriod) {
	if (freshnessPeriod < 0)
		usage();
	m_freshnessPeriod = time::milliseconds(freshnessPeriod);
}

/**
 * @fn Poke::setTimeout(int timeout)
 * @brief ndn method
 * @brief set producer's waitting time if necessary.
 * @param timeout, milliseconds 
 */
void Poke::setTimeout(int timeout) {
	if (timeout < 0)
		usage();
	m_timeout = time::milliseconds(timeout);
}

/**
 * @fn Poke::setPrefixName(char* prefixName)
 * @brief ndn method
 * @brief set root interest name to be listened, if coming interest matches this root, answer it.
 * @param prefixName, root interest name 
 * @note input string must statistify "ndn:/place" 
 */
void Poke::setPrefixName(char* prefixName) {
	m_prefixName = Name(prefixName);
}

/**
 * @fn Poke::isDataSent()
 * @brief tell other module whether send a packet successfully.
 * @return true  - data send successfully.
 * @return false - data send falsed.
 */
bool Poke::isDataSent() const	{
	return m_isDataSent;
}

/**
 * @fn Poke::onTimeout(const Interest& interest)
 * @brief ndn method
 * @brief function which is part of send interest, meaningless here.
 * @brief because the interest producer sent is only to tell others the data information, don't want to any back data.
 * @param interest, contains data information
 */
void Poke::onTimeout(const Interest& interest) {
	;
}

/**
 * @fn Poke::onData(const Interest& interest, Data& data)
 * @brief ndn method
 * @brief function which is part of send interest, meaningless here.
 * @brief because the interest producer sent is only to tell others the data information, don't want to any back data.
 * @brief will never call this function.
 */
void Poke::onData(const Interest& interest, Data& data) {
	;
}

/**
 * @fn Poke::createDataPacket(Name GetInterestName, int SearchResult, std::string aimAtPosition)
 * @brief ndn method
 * @brief function to create data packet according to information from map module and attributes.
 * @param GetInterestName, the interest name which is the same to the interest.
 * @param SearchResult, the information from searching the map module.
 * @param aimAtPosition, aim location.
 * @return the data packet.
 * @note payload statistify "1/74/20160511T089022.123456/Car1/"
 */
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

/**
 * @fn Poke::GetInformationFromMemory(int AimPosition)
 * @brief search map module and find the state of the position there.
 * @return the state of the aim position.
 */
int Poke::GetInformationFromMemory(int AimPosition) {
	int SearchResult = SEARCH_MAP_FAILED;
	try {
		SearchResult = Util::getCar().getMap().getMapAtPosition(AimPosition);
	} catch (std::exception& e) {
		SearchResult = SEARCH_MAP_FAILED;
	}
	return SearchResult;
}

/**
 * @fn Poke::GetAimPosition(std::string getName)
 * @brief get aim position from interest's name.
 * @return aim position.
 */
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

/**
 * @fn Poke::GetTypeOfInterest(std::string AimPosition, Interest interest)
 * @brief judget the type of the interest and make decision, common interest should not answer it.
 * @param AimPosition, aim position.
 * @param interest, coming interest.
 * @return the type of interest.
 */
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

/**
 * @fn Poke::BroadcastData(int AimPosition)
 * @brief broadcast a interest whose name is data information.
 * @param AimPosition, aim position.
 * @note this kind of interest is "/place/74/common/1/74/20160511T089022.123456/Car1/"
 */
void Poke::BroadcastData(int AimPosition) {
	std::string str = Util::int2string(AimPosition);
	std::string interest_name = "/place/" + str + "/common/" + m_payload;
	usepeek(interest_name);	
}

/**
 * @fn Poke::onInterest(const Name& name, const Interest& interest)
 * @brief ndn method
 * @brief receive coming interest matches root interest.
 * @param name, interest name.
 * @param interest, coming interest
 */
void Poke::onInterest(const Name& name, const Interest& interest) {
	m_isDataSent = true;		
	Name GetInterestName = interest.getName();
	std::string getName = GetInterestName.toUri();
	int AimPosition = GetAimPosition(getName);
	std::string AimPosStr = Util::int2string(AimPosition);
	int Type = GetTypeOfInterest(AimPosStr,  interest);
	int SearchResult;
	point getCurrentPosition = Util::getCar().getMap().getCurrentPosition();
	int currentPosition = getCurrentPosition.x * 10 + getCurrentPosition.y;
	if (currentPosition == AimPosition) {
		SearchResult = 1;
	}
	else {
		SearchResult = GetInformationFromMemory(AimPosition);	
	}

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

/**
 * @fn Poke::onRegisterFailed(const Name& prefix, const std::string& reason)
 * @brief ndn method
 * @brief call the function if register false.
 * @param prefix
 * @param reason
 */
void Poke::onRegisterFailed(const Name& prefix, const std::string& reason) {
	std::cerr << "Prefix Registration Failure." << std::endl;
	std::cerr << "Reason = " << reason << std::endl;
}

/**
 * @fn Poke::run()
 * @brief ndn method
 * @brief first here, the loop continue to listen the interest.
 * @brief call other functions if necessary.
 */
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

/**
 * @fn Poke::usepeek(std::string m_interest_name)
 * @brief use peek to send a interest whose name is data information
 * @brief call other functions if necessary.
 * @note input string must satisfy "/place/74/common/1/74/20160511T089022.123456/Car1/"
 */
void Poke::usepeek(std::string m_interest_name) {
	m_face.expressInterest(createInterestPacket(m_interest_name),
                     bind(&Poke::onData, this, _1, _2),
                     bind(&Poke::onTimeout, this, _1));
}

/**
 * @fn Poke::createInterestPacket(std::string m_name)
 * @brief create the interest packet according to the interest name and attributes. 
 * @param std::string m_name, the data information has sent before.
 * @return the interest packet.
 * @note input string must statistify "/place/74/common/1/74/20160511T089022.123456/Car1/"
 */
Interest Poke::createInterestPacket(std::string m_name) {
	Name interestName(m_name);
	Interest interestPacket(interestName);
	interestPacket.setMustBeFresh(true);
	interestPacket.setInterestLifetime(time::seconds(4));
	return interestPacket;
}