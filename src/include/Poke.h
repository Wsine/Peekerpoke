/**
 * @file Poke.h
 * @brief The declaration of class Poke
 */

#ifndef POKE_H_
#define POKE_H_

#include "../../lib/ndn-tools/common.hpp"
// #include "../../lib/ndn-tools/version.hpp"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "time.h"
#include "Util.h" // useless in Linux; Stupid!!!

using namespace ndn;

#define SEARCH_MAP_FAILED 9

class Poke : boost::noncopyable {
private:
	std::string m_payload;
	std::string m_carName;
	KeyChain m_keyChain;
	bool m_isForceDataSet;
	bool m_isUseDigestSha256Set;
	shared_ptr<Name> m_identityName;
	bool m_isLastAsFinalBlockIdSet;
	time::milliseconds m_freshnessPeriod;
	time::milliseconds m_timeout;
	Name m_prefixName;
	bool m_isDataSent;
	Face m_face;
/* Method */
public:
	explicit Poke();
	Poke(std::string carName);
	void usage();
	void setForceData();
	void setUseDigestSha256();
	void setIdentityName(char* identityName);
	void setLastAsFinalBlockId();
	void setFreshnessPeriod(int freshnessPeriod);
	void setTimeout(int timeout);
	void setPrefixName(char* prefixName);
	bool isDataSent() const;
	void onTimeout(const Interest& interest);
	void onData(const Interest& interest, Data& data);
	shared_ptr<Data> createDataPacket(Name GetInterestName, int SearchResult, std::string aimAtPosition);
	int GetInformationFromMemory(int AimPosition);
	int GetAimPosition(std::string getName);
	int GetTypeOfInterest(std::string AimPosition, Interest interest);
	void BroadcastData(int AimPosition);
	void onInterest(const Name& name, const Interest& interest);
	void onRegisterFailed(const Name& prefix, const std::string& reason);
	void run();
	void usepeek(std::string m_interest_name);
	Interest createInterestPacket(std::string m_name);
};

#endif