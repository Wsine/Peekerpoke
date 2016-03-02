#include "../include/Peek.h"

Peek::Peek() 
		: thisCarNumber("Car1")
		, m_mustBeFresh(false)
		, m_isPayloadOnlySet(false)
		, m_isChildSelectorRightmost(false)
		, m_isDataReceived(false)
		, m_timeout(-1)
		, m_ttl(-1)
		, m_minSuffixComponents(-1)
		, m_maxSuffixComponents(-1)
		, m_interestLifetime(-1) {
}
void Peek::usage() {
	printf("\n Usage:\n [-f] [-r] [-m min] [-M max] [-l lifetime] [-p] [-w timeout] ndn:/name\n"
      "   Get one data item matching the name prefix and write it to stdout\n"
      "   [-f]          - set MustBeFresh\n"
      "   [-r]          - set ChildSelector to select rightmost child\n"
      "   [-m min]      - set MinSuffixComponents\n"
      "   [-M max]      - set MaxSuffixComponents\n"
      "   [-l lifetime] - set InterestLifetime in time::milliseconds\n"
      "   [-p]          - print payload only, not full packet\n"
      "   [-w timeout]  - set Timeout in time::milliseconds\n"
      "   [-v]          - verbose output\n"
      "   [-h]          - print help and exit\n"
      "   [-V]          - print version and exit\n"
      "\n");
}
Peek::~Peek() {
	;
}

void Peek::setMustBeFresh() {
	m_mustBeFresh = true;
}

void Peek::setRightmostChildSelector() {
	m_isChildSelectorRightmost = true;
}

void Peek::setMinSuffixComponents(int minSuffixComponents) {
	if (minSuffixComponents < 0)
		usage();
	m_minSuffixComponents = minSuffixComponents;
}

void Peek::setMaxSuffixComponents(int maxSuffixComponents) {
	if (maxSuffixComponents < 0)
		usage();
	m_maxSuffixComponents = maxSuffixComponents;
}

void Peek::setInterestLifetime(int interestLifetime) {
	if (interestLifetime < 0)
		usage();
	m_interestLifetime = time::milliseconds(interestLifetime);
}

void Peek::setPayloadOnly() {
	m_isPayloadOnlySet = true;
}

void Peek::setTimeout(int timeout) {
	if (timeout < 0)
		usage();
	m_timeout = time::milliseconds(timeout);
	m_ttl = timeout;
}

void Peek::setPrefixName(std::string prefixName) {
	m_prefixName = prefixName;
	if (m_prefixName.length() == 0)
		usage();
}

time::milliseconds Peek::getDefaultInterestLifetime() {
	return time::seconds(4);
}

void Peek::onTimeout(const Interest& interest) {
	;
}

bool Peek::isDataReceived() {
	return m_isDataReceived;
}

Interest Peek::createInterestPacket(std::string m_name) {
	Name interestName(m_prefixName + m_name);
	Interest interestPacket(interestName);
	interestPacket.setMustBeFresh(m_mustBeFresh);
	interestPacket.setChildSelector(m_isChildSelectorRightmost);
	interestPacket.setMinSuffixComponents(m_minSuffixComponents);
	interestPacket.setMaxSuffixComponents(m_maxSuffixComponents);
	interestPacket.setInterestLifetime(m_interestLifetime);
	return interestPacket;
}

void Peek::CollectCurrentCarNumber(const Block& block)  {
	std::string data = reinterpret_cast<const char*>(block.value());
	char a = '#';
	data.resize(block.value_size(),  a);
	unsigned int loc = data.find("Car");
	int i;
	if  (loc != std::string::npos)  {
		for (i = loc; i < data.size() ; i++)  {
			haveReceived += data[i];
		}
		model = haveReceived;
	}
	else  {
		printf("We didn't find car number in the payload of the data packet,  we regarded it as a broadcast packet.\n");
		model = "broadcast";
	}
}

std::string Peek::getDataString(const Block& block) {
	std::string get_inf = reinterpret_cast<const char*>(block.value());
	char a = '#';
	get_inf.resize(block.value_size(),  a);
	unsigned int loc = get_inf.find("Car");
	std::string result = "";
	if  (loc != std::string::npos)  {
		int i;
		for (i = 0 ; i <= loc - 1; i++)  {
			result += get_inf[i];
		}
	}
	else  {
		printf("Shold not execute\n");
		//std::cout << "Should not execute here!" << std::endl << std::endl; 
	}
	return result;
}
void Peek::onData(const Interest& interest, Data& data) {
	m_isDataReceived = true;
	if (m_isPayloadOnlySet) {
		const Block& block = data.getContent();
		//Name GetInterestName = interest.getName();
		m_received_data = getDataString(block);
		CollectCurrentCarNumber(block);
	}
	else {
		const Block& block = data.wireEncode();
		//std::cout.write(reinterpret_cast<const char*>(block.wire()), block.size());
	}	
}

void Peek::run() {
	haveReceived = "/filter/" + thisCarNumber + "/";
	model = "/filter/" + thisCarNumber + "/";
	try {
		while (1) {
			sleep(1);
			m_ttl--;
			if (m_ttl != 0) {
				std::cout << "Current model: " << model << std::endl;
				//printf("Current model: %s\n", model);
				m_face.expressInterest(createInterestPacket(model), bind(&Peek::onData, this, _1, _2), bind(&Peek::onTimeout, this, _1));
				m_face.processEvents();
			}
			else  {
				haveReceived = "";
				break;
			}
		}		
	}
	catch (std::exception& e) {
		std::cerr << "ERROR: " << e.what() << "\n" << std::endl;
	}
}

std::string Peek::getReceivedData() {
	return m_received_data;
}