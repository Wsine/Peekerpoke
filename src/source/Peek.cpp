#include "../include/Peek.h"

NdnPeek::NdnPeek() {
	m_mustBeFresh = true;
	m_isPayloadOnlySet = true;
	m_isChildSelectorRightmost = false;
	m_timeout = time::milliseconds(3);
	m_ttl = 3;
	m_minSuffixComponents = -1;
	m_maxSuffixComponents = -1;
	m_interestLifetime = -1;
}
NdnPeek::~NdnPeek() {
	;
}
void NdnPeek::setMustBeFresh() {
	m_mustBeFresh = true;
}
void NdnPeek::setRightmostChildSelector() {
	m_isChildSelectorRightmost = true;
}
void NdnPeek::setMinSuffixComponents(int minSuffixComponents) {
	if (minSuffixComponents < 0)
		usage();
	m_minSuffixComponents = minSuffixComponents;
}
void NdnPeek::setMaxSuffixComponents(int maxSuffixComponents) {
	if (maxSuffixComponents < 0)
		usage();
	m_maxSuffixComponents = maxSuffixComponents;
}
void NdnPeek::setInterestLifetime(int interestLifetime) {
	if (interestLifetime < 0)
		usage();
	m_interestLifetime = time::milliseconds(interestLifetime);
}
void NdnPeek::setPayloadOnly() {
	m_isPayloadOnlySet = true;
}
void NdnPeek::setTimeout(int timeout) {
	if (timeout < 0)
		usage();
	m_timeout = time::milliseconds(timeout);
	m_ttl = timeout;
}
void NdnPeek::setPrefixName(char* prefixName) {
	m_prefixName = prefixName;
	if (m_prefixName.length() == 0)
		usage();
}
time::milliseconds NdnPeek::getDefaultInterestLifetime() {
	return time::seconds(4);
}
void NdnPeek::onTimeout(const Interest& interest) {
	;
}
bool NdnPeek::isDataReceived() const {
	return m_isDataReceived;
}
Interest NdnPeek::createInterestPacket(std::string m_name) {
	Name interestName(m_prefixName + m_name);
	Interest interestPacket(interestName);
	interestPacket.setMustBeFresh(m_mustBeFresh);
	interestPacket.setChildSelector(m_isChildSelectorRightmost);
	interestPacket.setMinSuffixComponents(m_minSuffixComponents);
	interestPacket.setMaxSuffixComponents(m_maxSuffixComponents);
	interestPacket.setInterestLifetime(m_interestLifetime);
	return interestPacket;
}
void NdnPeek::CollectCurrentCarNumber(const Block& block)  {
	std::string data = reinterpret_cast<const char*>(block.value());
	char a = '#';
	data.resize(block.value_size(),  a);
	unsigned int loc = data.find("car");
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
void NdnPeek::onData(const Interest& interest, Data& data) {
	m_isDataReceived = true;
	if (m_isPayloadOnlySet) {
		const Block& block = data.getContent();
		Name GetInterestName = interest.getName();
		m_received_data = GetInterestName.toUri();
		CollectCurrentCarNumber(block);
	}
	else {
		const Block& block = data.wireEncode();
		//std::cout.write(reinterpret_cast<const char*>(block.wire()), block.size());
	}	
}

void NdnPeek::run(std::string carName) {
	isBroadcast = true;
	haveReceived = "/filter/" + carName + "/";
	model = "/filter/" + carName + "/";
	try {
		while (1) {
			sleep(1);
			m_ttl--;
			if (m_ttl != 0) {
				if (isBroadcast == true) {
					printf("Current model: %s\n", model);
					m_face.expressInterest(createInterestPacket(model), bind(&NdnPeek::onData, this, _1, _2), bind(&NdnPeek::onTimeout, this, _1));
					m_face.processEvents();
					isBroadcast = false;
				}
				else  {
					printf("Current model: %s\n", model);
					m_face.expressInterest(createInterestPacket(model), bind(&NdnPeek::onData, this, _1, _2), bind(&NdnPeek::onTimeout, this, _1));
					m_face.processEvents();
				}	
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
std::string NdnPeek::getReceivedData() {
	return m_received_data;
}
