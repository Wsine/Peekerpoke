/**
 * @file Peek.cpp
 * @brief The implementation of class Peek
 */

#include "../include/Peek.h"

/**
 * @fn Peek::Peek()
 * @brief Default constructor of class Peek
 * @brief set the attributes of the interest packet and consumer, has been replaced by Peek::Peek(std::string carName)
 */
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

/**
 * @fn Peek::Peek(std::string carName)
 * @brief Default constructor of class Peek
 * @brief set the attributes of the interest packet and consumer, set car name directly.
 * @param carName the unique ID of the car
 */
Peek::Peek(std::string carName) 
	: thisCarNumber(carName)
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

/**
 * @fn Peek::usage()
 * @brief introduce kinds of attributes.
 */
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

/**
 * @fn Peek::~Peek()
 */
Peek::~Peek() {
	;
}

/**
 * @fn Peek::usetMustBeFresh()
 * @brief ndn method
 * @brief set the fresh attribute of the interest.
 * @brief the interest packet should always be fresh when received by producer, otherwise it will be discarded.
 */
void Peek::setMustBeFresh() {
	m_mustBeFresh = true;
}

/**
 * @fn Peek::setRightmostChildSelector()
 * @brief ndn method
 * @brief set the Right most Child Selector attribute of the interest.
 * @brief producer will select the most right child pf the prefix name.
 */
void Peek::setRightmostChildSelector() {
	m_isChildSelectorRightmost = true;
}

/**
 * @fn Peek::setMinSuffixComponents(int minSuffixComponents)
 * @brief ndn method
 * @brief set the minist component attribute of the interest.
 * @param minSuffixComponents
 */
void Peek::setMinSuffixComponents(int minSuffixComponents) {
	if (minSuffixComponents < 0)
		usage();
	m_minSuffixComponents = minSuffixComponents;
}

/**
 * @fn Peek::setMaxSuffixComponents(int maxSuffixComponents)
 * @brief ndn method
 * @brief set the max component attribute of the interest.
 * @param maxSuffixComponents
 */
void Peek::setMaxSuffixComponents(int maxSuffixComponents) {
	if (maxSuffixComponents < 0)
		usage();
	m_maxSuffixComponents = maxSuffixComponents;
}

/**
 * @fn Peek::setInterestLifetime(int interestLifetime)
 * @brief ndn method
 * @brief set the interest packet life time in case too many packets in the network.
 * @param interestLiftetime, milliseconds
 */
void Peek::setInterestLifetime(int interestLifetime) {
	if (interestLifetime < 0)
		usage();
	m_interestLifetime = time::milliseconds(interestLifetime);
}

/**
 * @fn Peek::setPayloadOnly()
 * @brief ndn method
 * @brief set the interest packet attribute, analysis the payload only when receiving data.
 */
void Peek::setPayloadOnly() {
	m_isPayloadOnlySet = true;
}

/**
 * @fn Peek::setTimeout(int timeout)
 * @brief ndn method
 * @brief set waitting time and m_ttl for the consumer, will execute onTimeout if it happens.
 * @param timeout, milliseconds 
 */
void Peek::setTimeout(int timeout) {
	if (timeout < 0)
		usage();
	m_timeout = time::milliseconds(timeout);
	m_ttl = timeout;
}

/**
 * @fn Peek::setPrefixName(std::string prefixName)
 * @brief ndn method
 * @brief set the main part of the interest name.
 * @param std::string prefixName, input string.
 * @note input string must statistify "ndn:/place/74" 
 */
void Peek::setPrefixName(std::string prefixName) {
	m_prefixName = prefixName;
	if (m_prefixName.length() == 0)
		usage();
}

/**
 * @fn Peek::getDefaultInterestLifetime()
 * @brief ndn method
 * @brief set default interest life time, 4 seconds.
 */
time::milliseconds Peek::getDefaultInterestLifetime() {
	return time::seconds(4);
}

/**
 * @fn Peek::onTimeout(const Interest& interest)
 * @brief ndn method
 * @brief hasn't received data back within timeout, execute this.
 * @brief we don't need do something about it because we have implemented a parameter called m_isDataoReceived to tell planner module the result.
 * @param the interest has been sent  
 */
void Peek::onTimeout(const Interest& interest) {
	;
}

/**
 * @fn Peek::isDataReceived()
 * @brief tell planner module whether received data from other cars. 
 * @return true  - received data through sending interest.
 * @return false - Did not receive data back.
 */
bool Peek::isDataReceived() {
	return m_isDataReceived;
}

/**
 * @fn Peek::createInterestPacket(std::string m_name)
 * @brief ndn method
 * @brief create the interest packet according to the interest name and attributes we have set. 
 * @param std::string m_name, the part of interest name, actually the information of received car filter and time stamp.
 * @return the interest packet.
 * @note input string must statistify "/filter/Car1/Car2/Car3/123456" 
 */
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

/**
 * @fn Peek::CollectCurrentCarNumber(const Block& block)
 * @brief analysis the coming data, find producer's car ID from it.
 * @brief add producer's car ID into received car filter so as not to receive same data.  
 * @param Block& block, contain coming data.
 */
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
		currentFilterCarNumber = haveReceived;
	}
	else  {
		printf("Did not find producer's Car number in the payload of data.\n");
		currentFilterCarNumber = "broadcast";
	}
}

/**
 * @fn Peek::getDataString(const Block& block)
 * @brief convert coming data block to string.  
 * @param Block& block, contain coming data.
 * @return the data in the form of string.
 */
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
		printf("Did not find producer's Car number in the payload of data.\n");
	}
	return result;
}

/**
 * @fn Peek::onData(const Interest& interest, Data& data)
 * @brief ndn method
 * @brief received data, and add its producer to received car filter.  
 * @brief set m_isDataReceived true.
 * @param interest, the interest has been sent
 * @param data, coming data
 */
void Peek::onData(const Interest& interest, Data& data) {
	m_isDataReceived = true;
	if (m_isPayloadOnlySet) {
		const Block& block = data.getContent();
		m_received_data = getDataString(block);
		CollectCurrentCarNumber(block);
	}
	else {
		const Block& block = data.wireEncode();
		//std::cout.write(reinterpret_cast<const char*>(block.wire()), block.size());
	}	
}

/**
 * @fn Peek::run()
 * @brief ndn method
 * @brief the loop continue to send interest, receive data and modify received car filter within m_ttl.  
 * @brief will execute here first and call other functions if necessary.
 */
void Peek::run() {
	haveReceived = "/filter/" + thisCarNumber + "/";
	currentFilterCarNumber = "/filter/" + thisCarNumber + "/";
	try {
		while (1) {
			sleep(1);
			m_ttl--;
			if (m_ttl != 0) {
				printf("Current currentFilterCarNumber =  %s\n", currentFilterCarNumber.c_str());
				std::string getTime = time::toIsoString(time::system_clock::now());
				std::string timeStamp = getTime.substr(16, 21);
				m_face.expressInterest(createInterestPacket(currentFilterCarNumber + timeStamp), bind(&Peek::onData, this, _1, _2), bind(&Peek::onTimeout, this, _1));
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

/**
 * @fn Peek::getReceivedData()
 * @brief pass the data information to planner module.
 * @return information of the aim location from other cars.  
 */
std::string Peek::getReceivedData() {
	return m_received_data;
}