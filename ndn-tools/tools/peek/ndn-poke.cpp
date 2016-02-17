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
#include <sstream>
#include "time.h"
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

namespace ndn {
namespace peek {
	class NdnPoke : boost::noncopyable {
		private:
			std::string m_payload;
			std::string thisCarNumber;
			KeyChain m_keyChain;
			bool ifUnknown;
			std::string m_programName;
			bool m_isForceDataSet;
			bool m_isUseDigestSha256Set;
			shared_ptr<Name> m_identityName;
			bool m_isLastAsFinalBlockIdSet;
			time::milliseconds m_freshnessPeriod;
			time::milliseconds m_timeout;
			Name m_prefixName;
			bool m_isDataSent;
			Face m_face;
		public:explicit
		NdnPoke(char* programName)
		: m_programName(programName)
		, m_isForceDataSet(false)
		, m_isUseDigestSha256Set(false)
		, m_isLastAsFinalBlockIdSet(false)
		, m_freshnessPeriod(-1)
		, m_timeout(-1)
		, m_isDataSent(false)
		{}

		void usage() {
			std::cout << "NO.2 Car" << std::endl;
			exit(1);
		}
		void setForceData() {
			m_isForceDataSet = true;
		}
		void setUseDigestSha256() {
			m_isUseDigestSha256Set = true;
		}  
		void setIdentityName(char* identityName) {
			m_identityName = make_shared<Name>(identityName);
		}  
		void setLastAsFinalBlockId() {
			m_isLastAsFinalBlockIdSet = true;
		}
		void setFreshnessPeriod(int freshnessPeriod) {
			if (freshnessPeriod < 0)
				usage();
			m_freshnessPeriod = time::milliseconds(freshnessPeriod);
		}  
		void setTimeout(int timeout) {
			if (timeout < 0)
				usage();
			m_timeout = time::milliseconds(timeout);
		}
		void setPrefixName(char* prefixName) {
			m_prefixName = Name(prefixName);
		}
		time::milliseconds getDefaultTimeout() {
			return time::seconds(10);
		}
		bool isDataSent() const	{
			return m_isDataSent;
		}
		void onTimeout(const Interest& interest) {
			//std::cout << "Should not execute here." << std::endl << std::endl;
		}
		void onData(const Interest& interest, Data& data) {
			;
		}
	
		/** Modified by LiBoyang on Feb 8th,  2016 
		 *  This function is used to read the information from file and make it as a data packet.
		 *  @return the data packet
		 */
		shared_ptr<Data> createDataPacket(Name GetInterestName,  int SearchResult) {
			auto dataPacket = make_shared<Data>(GetInterestName);
			std::string timeStamp = time::toIsoString(time::system_clock::now());
			std::string payload;
			std::stringstream ss;
			ss << SearchResult;
			ss >> payload;
			payload += "/",  payload += timeStamp,  payload += "/",  payload += thisCarNumber,  payload += "/";
			m_payload = payload;
			unsigned int loc = payload.find("unknown");
			/* The information is unknown,  we don't send it. */
			if (loc != std::string::npos) {
				ifUnknown = true;
			}
			else {
				ifUnknown = false;
			}
			dataPacket->setContent(reinterpret_cast<const uint8_t*>(payload.c_str()),  payload.length());
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
	
		/** Modified by LiBoyang on Feb 9th,  2016
		 *  This funnction is used to return the place state stored in this car Map memory.
		 *  @param			AimPosition The aim position to be searched.
		 *  @return 9 if search failed, the state of the map otherwise.
		 */
		int GetInformationFromMemory(int AimPosition) {		
			std::cout << "Aim position is: " << AimPosition << std::endl;
			int SearchResult = SearchMyMap(AimPosition);
			if (SearchResult == -1) {
				std::cout << "We don't have the information." << std::endl;
				return 9;
			}
			std::cout << "Search successfully." << std::endl;
			return SearchResult;
		}
	
		/** Modified by LiBoyang on Feb 9th,  2016
		 *	@param			getName The position get return.
		 *  @return the postion in integer.
		 */
		int GetAimPosition(std::string getName)  {
			char PlaceNumber[2];
			PlaceNumber[0] = getName[7];
			PlaceNumber[1] = getName[8];
			//std::cout << "AimPosition:" << PlaceNumber[0] << ", " << PlaceNumber[1] << std::endl;
			if (PlaceNumber[1] == '/') {
				PlaceNumber[1] = PlaceNumber[0];
				PlaceNumber[0] = '0';
			} 
			int AimPosition = (PlaceNumber[0] - '0') * 10 + PlaceNumber[1] - '0';
			return AimPosition;
		}
		/** Modified by LiBoyang on Feb 10th,  2016
		 *	@param			The position get return.
		 *  @return the postion in string.
		 */
		std::string GetAimPositionString(int pos)  {
			std::stringstream ss;
			std::string str;
			ss << pos;
			ss >> str;
			return str;
		}
				
		/** Modified by LiBoyang on Feb 9th,  2016
		 *	This function is used to return the type of the interest: broadcast, filter, common.
		 *	@param			AimPosition
		 *	@param			interest Interest received.
		 *	@return 1 if broadcast model.
		 *	@return 2 if filter model.
		 *	@return 3 if common model.
		 *	@return -1 default.
		 */
		int GetTypeOfInterest(std::string AimPosition,  Interest interest) {
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
			std::cout << init_place << std::endl;
			return result;
		}
	
		/** Modified by LiBoyang on Feb 9th,  2016
		 *	This function is used to broadcast a data in the form of interest.
		 *	Even some cars didn't send the interest, they could also get the information in the form of interest, and these
		 *  information may be helpful.
		 *	@param			AimPosition
		 */
		void BroadcastData(int AimPosition)  {
			std::stringstream ss;
			std::string str;
			ss << AimPosition;
			ss >> str;
			std::string interest_name = "/place/" + str + "/common/" + m_payload;
			usepeek(interest_name);	
		}
		
		/** Modified by LiBoyang on Feb 9th,  2016
		 *	Get the interest, we should do something according to the interest.
		 *  First we should judge the type of the interest, and get the aim position which consumer interests, then search it in our memory map.
		 *  Respond it or do nothing.
		 *	@param	interest	Interest we received.
		 */
		void onInterest(const Name& name, const Interest& interest) {
			m_isDataSent = true;		
			Name GetInterestName = interest.getName();
			std::string getName = GetInterestName.toUri();
			int AimPosition = GetAimPosition(getName);
			std::string AimPosStr = GetAimPositionString(AimPosition);
			int Type = GetTypeOfInterest(AimPosStr,  interest);
			int SearchResult =  GetInformationFromMemory(AimPosition);
		
			switch(Type) {
				/* The broadcast model interst,  we should respond it with our data. */
				case 1: {
					std::cout << "Receive interest. The interest is broadcast model. We should send the data." << std::endl << std::endl;
					if (SearchResult != 9) {
						shared_ptr<Data> dataPacket = createDataPacket(GetInterestName,  SearchResult);
						m_face.put(*dataPacket);
						BroadcastData(AimPosition);
					}
					else {
						std::cout << "Search information error." << std::endl;
					}
					break;
				}
				/* We get the interest which is filtered. We should judge whether our car has been banned. */
				case 2: {
					/* To search if our car number is in the filter. */
					unsigned int location = getName.find(thisCarNumber,  0);
					if (location != std::string::npos) {
						std::cout << "The information from this car has been received,  we don't need to send it." << std::endl;
					}
					else {
						std::cout << "We are not in the filter,  we should send the data again." << std::endl;
						if (SearchResult != 9) {
							shared_ptr<Data> dataPacket = createDataPacket(GetInterestName,  SearchResult);
							m_face.put(*dataPacket);
							BroadcastData(AimPosition);
						}
						else {
							std::cout << "Search information error." << std::endl;
						}
					}
					break;
				}
					/* This kind of interest contains data information. Modified by LiBoyang on Feb 9th,  2016. */
				case 3: {
					std::cout << "Someone has sent a data packet: " << getName << std::endl;
					break;
				}
				default: {
					std::cout << "Type error!" << std::endl;
					break;
				}
			}
		}
  
		void onRegisterFailed(const Name& prefix, const std::string& reason) {
			std::cerr << "Prefix Registration Failure." << std::endl;
			std::cerr << "Reason = " << reason << std::endl;
		}

		/** Modified by LiBoyang on August 5th,  2015
		 *  I have modified it for the time of processEvents.
		 */
		void run() {
			thisCarNumber = "car2";
			try {
				m_face.setInterestFilter(m_prefixName,
                                 bind(&NdnPoke::onInterest, this, _1, _2),
                                 RegisterPrefixSuccessCallback(),
                                 bind(&NdnPoke::onRegisterFailed, this, _1, _2));
				m_face.processEvents();
			}
			catch (std::exception& e) {
				std::cerr << "ERROR: " << e.what() << "\n" << std::endl;
				exit(1);
			}
		}
	
		/** Modified by LiBoyang on August 19th,  2015
		 *  Help to broadcast the data in the form of interest.
		 */
		void usepeek(std::string m_interest_name) {
			m_face.expressInterest(createInterestPacket(m_interest_name),
                             bind(&NdnPoke::onData, this, _1, _2),
                             bind(&NdnPoke::onTimeout, this, _1));
		}
	
		/** Modified by LiBoyang on August 19th,  2015 
		 *  Help to broadcast the data in the form of interest.
		 */
		Interest createInterestPacket(std::string m_name) {
			Name interestName(m_name);
			Interest interestPacket(interestName);
			interestPacket.setMustBeFresh(true);
			interestPacket.setInterestLifetime(time::seconds(4));
			return interestPacket;
		}
	
		/** Added by LiBoyang on Feb 4th 
		 *  When we get a interest from other cars, we should search the shared memory to provide the data.
		 *  This function is used to search the shared memory.
		 *  Return the state of the interest area(0 or 1), returns -1 if failed. 
		 */
		int SearchMyMap(int AimPosition) {
			int shm_id;
			key_t key;
			int *MapMemory;
			char pathname[30] ;
			strcpy(pathname,"/tmp") ;
			key = ftok(pathname,0x03);
			if (key == -1) {
				perror("ftok error");
				return -1;
			}
			shm_id = shmget(key,0, 0);   
			if (shm_id == -1) {
				perror("shmget error");
				return -1;
			}
			MapMemory = (int*)shmat(shm_id,NULL,0);
			return MapMemory[AimPosition];
			if (shmdt(MapMemory) == -1) {
				perror("detach error\n");
				return -1;
			}
		} 
	};

	int main(int argc, char* argv[]) {
		int option;
		NdnPoke program(argv[0]);
		while ((option = getopt(argc, argv, "hfDi:Fx:w:V")) != -1) {
			switch (option) {
			case 'h':
				program.usage();
				break;
			case 'f':
				program.setForceData();
				break;
			case 'D':
				program.setUseDigestSha256();
				break;
			case 'i':
				program.setIdentityName(optarg);
				break;
			case 'F':
				program.setLastAsFinalBlockId();
				break;
			case 'x':
				program.setFreshnessPeriod(atoi(optarg));
				break;
			case 'w':
				program.setTimeout(atoi(optarg));
				break;
			case 'V':
				std::cout << "ndnpoke " << tools::VERSION << std::endl;
				return 0;
			default:
				program.usage();
				break;
			}
		}

		argc -= optind;
		argv += optind;
		if (argv[0] == 0) {
			program.usage();
		}
		program.setPrefixName(argv[0]);
		program.run();
		if (program.isDataSent()) {
			return 0;
		}
		else {
			return 1;
		}
	}
} 
}

int main(int argc, char** argv) {
  	return ndn::peek::main(argc, argv);
}