#ifndef PEEK_H_
#define PEEK_H_

#include "../../lib/ndn-tools/version.hpp"
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

namespace ndn {
	namespace peek {
		class NdnPeek : boost :: noncopyable {
		private:
			std::string model;
			std::string haveReceived;
  			std::string m_programName;
			std::string m_prefixName;
			std::string m_received_data;
  			int m_ttl;
  			int m_minSuffixComponents;
  			int m_maxSuffixComponents;
  			bool isBroadcast;  			
  			bool m_mustBeFresh;
  			bool m_isChildSelectorRightmost;
  			bool m_isPayloadOnlySet;
  			bool m_isDataReceived;
  			time::milliseconds m_interestLifetime;
  			time::milliseconds m_timeout;
  			Face m_face;
  		/* Method */
		public:
			NdnPeek();
			~NdnPeek();
		/* ndn method */
		public:
			void usage();
			void setMustBeFresh();
			void setRightmostChildSelector();
			void setMinSuffixComponents(int minSuffixComponents);
			void setMaxSuffixComponents(int maxSuffixComponents);
			void setInterestLifetime(int interestLifetime);
			void setPayloadOnly();
			void setTimeout(int timeout);
			void setPrefixName(std::string prefixName);
			time::milliseconds getDefaultInterestLifetime();
			void onTimeout(const Interest& interest);
			bool isDataReceived();
		public:
			Interest createInterestPacket(std::string m_name);
			void collectCurrentCarNumber(const Block& block);
			void onData(const Interest& interest, Data& data);
			void run(std::string carName);
			void usage();
			std::string getReceivedData();
		};
	}
}


#endif