/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014-2015,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of ndn-tools (Named Data Networking Essential Tools).
 * See AUTHORS.md for complete list of ndn-tools authors and contributors.
 *
 * ndn-tools is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndn-tools is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndn-tools, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * Copyright (c) 2014,  Regents of the University of California,
 *                      Arizona Board of Regents,
 *                      Colorado State University,
 *                      University Pierre & Marie Curie, Sorbonne University,
 *                      Washington University in St. Louis,
 *                      Beijing Institute of Technology,
 *                      The University of Memphis
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Jerald Paul Abraham <jeraldabraham@email.arizona.edu>
 */

#include "core/version.hpp"
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

	class NdnPeek : boost::noncopyable {
		private:
			std::string thisCarNumber;
			std::string model;
			std::string haveReceived;
			int m_ttl;
			bool isBroadcast;
			std::string m_programName;
			bool m_mustBeFresh;
			bool m_isChildSelectorRightmost;
			int m_minSuffixComponents;
			int m_maxSuffixComponents;
			time::milliseconds m_interestLifetime;
			bool m_isPayloadOnlySet;
			time::milliseconds m_timeout;
			std::string m_prefixName;
			bool m_isDataReceived;
			Face m_face;

		public:explicit
			NdnPeek(char* programName)
			: m_programName(programName)
			, m_mustBeFresh(false)
			, m_isChildSelectorRightmost(false)
			, m_minSuffixComponents(-1)
			, m_maxSuffixComponents(-1)
			, m_interestLifetime(-1)
			, m_isPayloadOnlySet(false)
			, m_timeout(-1)
			, m_prefixName("")
			, m_isDataReceived(false) {}

			void usage() {
				std::cout << "NO.2 Car" << std::endl;
				exit(1);
			}
			void setMustBeFresh() {
				m_mustBeFresh = true;
			}
			void setRightmostChildSelector() {
				m_isChildSelectorRightmost = true;
			}
			void setMinSuffixComponents(int minSuffixComponents) {
				if (minSuffixComponents < 0)
					usage();
				m_minSuffixComponents = minSuffixComponents;
			}
			void setMaxSuffixComponents(int maxSuffixComponents) {
				if (maxSuffixComponents < 0)
					usage();
				m_maxSuffixComponents = maxSuffixComponents;
			}
			void setInterestLifetime(int interestLifetime) {
				if (interestLifetime < 0)
					usage();
				m_interestLifetime = time::milliseconds(interestLifetime);
			}
			void setPayloadOnly() {
				m_isPayloadOnlySet = true;
			}
			void setTimeout(int timeout) {
				if (timeout < 0)
					usage();
				m_timeout = time::milliseconds(timeout);
				m_ttl = timeout;
			}
			void setPrefixName(char* prefixName) {
				m_prefixName = prefixName;
				if (m_prefixName.length() == 0)
					usage();
			}
			time::milliseconds getDefaultInterestLifetime() {
				return time::seconds(4);
			}
			void onTimeout(const Interest& interest) {
				;
			}
			bool isDataReceived() const {
				return m_isDataReceived;
			}
			/* Modified by LiBoyang on August 10th,  2015 */
			Interest createInterestPacket(std::string m_name) {
				Name interestName(m_prefixName + m_name);
				Interest interestPacket(interestName);
				if (m_mustBeFresh)
					interestPacket.setMustBeFresh(true);
				if (m_isChildSelectorRightmost)
					interestPacket.setChildSelector(1);
				if (m_minSuffixComponents >= 0)
					interestPacket.setMinSuffixComponents(m_minSuffixComponents);
				if (m_maxSuffixComponents >= 0)
					interestPacket.setMaxSuffixComponents(m_maxSuffixComponents);
				if (m_interestLifetime < time::milliseconds::zero())
					interestPacket.setInterestLifetime(getDefaultInterestLifetime());
				else
					interestPacket.setInterestLifetime(m_interestLifetime);
				return interestPacket;
			}
			
			/**	Modified by LiBoyang on Feb 9th,  2016
			 *
			 */
			void CollectCurrentCarNumber(const Block& block)  {
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
					std::cout << "We didn't find car number in the payload of the data packet,  we regarded it as a broadcast packet." << std::endl;
					model = "broadcast";
				}
			}

			/** Modified by LiBoyang on Feb 9th,  2016
		     *	@param			getName The position get return.
		     *  @return the postion in integer.
		     */
			int GetAimPosition(std::string getName)  {
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
			
			/** Modified by LiBoyang on Feb 9th,  2016
			 *  This function is used to get the data content and return it in string form.
			 *	@param		block
			 *	@return		data in the form of string
			 */
			std::string GetDataString(const Block& block)  {
				std::string get_inf = reinterpret_cast<const char*>(block.value());
				char a = '#';
				get_inf.resize(block.value_size(),  a);
				unsigned int loc = get_inf.find("car");
				std::string result = "";
				if  (loc != std::string::npos)  {
					int i;
					for (i = 0 ; i <= loc - 1; i++)  {
						result += get_inf[i];
					}
				}
				else  {
					std::cout << "Should not execute here!" << std::endl << std::endl; 
				}
				return result;
			}
			
			/**	Modified by LiBoyang on Feb 9th,  2016
			  *	@param	inf
			  *	@return	received data information int
			  */
			int GetDataInt(std::string inf)  {
				char InfNumber = inf[0];
				int result = InfNumber - '0';
				return result;
			}
			
			/** Modified by LiBoyang on Feb 9th,  2016
			 *  This function is used to deal with the data received. We should update the memory map
             *  and then referesh received car number.
             */
			void onData(const Interest& interest, Data& data) {
				m_isDataReceived = true;
				if (m_isPayloadOnlySet) {
					const Block& block = data.getContent();
					Name GetInterestName = interest.getName();
					std::string getName = GetInterestName.toUri();
					int AimPosition = GetAimPosition(getName);
					std::string GetData_string = GetDataString(block);
					int GetData_int= GetDataInt(GetData_string);
					std::cout << "Received data: " << GetData_string << std::endl << std::endl;
					/* Update the MapInf.txt */
					int ifModifyMap = ModifyMap(AimPosition,  GetData_int);
					if (ifModifyMap != -1) {
						std::cout << "Modified successfully" << std::endl;
					}
					else {
						std::cout << "Modified error" << std::endl;
					}
					CollectCurrentCarNumber(block);  
					//std::cout << "Received data: " << std::endl;
					//std::cout.write(reinterpret_cast<const char*>(block.value()),  block.size()-1);
					//std::cout << std::endl << std::endl;
				}
				else {
					const Block& block = data.wireEncode();
					std::cout.write(reinterpret_cast<const char*>(block.wire()), block.size());
				}	
			}
						
			/** Added by LiBoyang on Feb 4th 
			 * When we get a data from other cars, we should modify our shared memory map.
			 * This function is used to modify the shared memory.
			 * Return 1 if success, -1 if failed. 
			 */
			int ModifyMap(int AimPosition, int ReceivedData) {
				std::cout << "Aim position: " << AimPosition << std::endl;
				std::cout << "Received data: " << ReceivedData << std::endl;
				int shm_id;
				key_t key;
				int *MapMemory;
				char pathname[30] ;
				char temp[8];
				strcpy(pathname,"/tmp") ;
				key = ftok(pathname,0x03);
				if (key == -1) {
					perror("ftok error");
					return -1;
				}
				shm_id = shmget(key,8192, IPC_CREAT|0600);   
				if (shm_id == -1) {
					perror("shmget error");
					return -1;
				}
				MapMemory = (int*)shmat(shm_id,  NULL, 0);
				memset(temp,  0x00,  sizeof(temp));
				strcpy(temp,  "test");
				MapMemory[AimPosition]= ReceivedData;
				shmdt(MapMemory);
				return 1;
			}
			void run() {
				isBroadcast = true;
				thisCarNumber = "car2";
				haveReceived = "/filter/"+thisCarNumber+"/";
				model = "/filter/"+thisCarNumber+"/";
				try {
					while (1) {
						sleep(1);
						m_ttl--;
						if (m_ttl != 0) {
							if (isBroadcast == true) {
								std::cout << "Current model: " << model << std::endl;
								m_face.expressInterest(createInterestPacket(model), bind(&NdnPeek::onData, this, _1, _2), bind(&NdnPeek::onTimeout, this, _1));
								m_face.processEvents();
								isBroadcast = false;
							}
							else  {
								std::cout << "Current model: " << model << std::endl;
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
					exit(1);
				}
			}	
			
	};

	int main(int argc, char* argv[]) {
		NdnPeek program(argv[0]);
		int option;
		while ((option = getopt(argc, argv, "hfrm:M:l:pw:V")) != -1) {
			switch (option) {
			case 'h':
				program.usage();
				break;
			case 'f':
				program.setMustBeFresh();
				break;
			case 'r':
				program.setRightmostChildSelector();
				break;
			case 'm':
				program.setMinSuffixComponents(atoi(optarg));
				break;
			case 'M':
				program.setMaxSuffixComponents(atoi(optarg));
				break;
			case 'l':
				program.setInterestLifetime(atoi(optarg));
				break;
			case 'p':
				program.setPayloadOnly();
				break;
			case 'w':
				program.setTimeout(atoi(optarg));
				break;
			case 'V':
				std::cout << "ndnpeek " << tools::VERSION << std::endl;
				return 0;
			default:
				program.usage();
				break;
			}
		}
		argc -= optind;
		argv += optind;
		if (argv[0] == 0)
			program.usage();
		program.setPrefixName(argv[0]);
		program.run();
		if (program.isDataReceived())
			return 0;
		else
			return 1;
	}

}
}

int main(int argc, char** argv) {
  return ndn::peek::main(argc, argv);
}