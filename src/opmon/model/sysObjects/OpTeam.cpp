#include "OpTeam.hpp"

namespace OpMon {
	namespace Model {

		OpTeam::OpTeam(std::string const &name) {
			this->name = name;
		}

		void OpTeam::heal() {
			for(int i = 0; i < nbreOfOp; i++) {
				if(opteam[i] != nullptr) {
					opteam[i]->heal(opteam[i]->getStatHP());
					opteam[i]->confus = false;
					opteam[i]->setStatus(Status::NOTHING);
				}
			}
		}

		bool OpTeam::addOpMon(OpMon *toAdd) {
			for(int i = 0; i < nbreOfOp; i++) {
				if(opteam[i] == nullptr) {
					opteam[i] = toAdd;
					return true;
				}
			}
			return false;
		}

		OpMon *OpTeam::removeOp(int number) {
			if(nbreOfOp == 1) { //If there is only one OpMon, return nullptr
				return nullptr;
			}
			OpMon *toReturn = opteam[number];
			opteam[number] = nullptr;
			for(int i = number; i < nbreOfOp; i++) {
				if(i != 5) {
					opteam[i] = opteam[i + 1];
				} else {
					opteam[5] = nullptr;
				}
			}
			return toReturn;
		}

		OpMon *OpTeam::operator[](int id) const {
			return opteam[id];
		}

		bool OpTeam::isKo() const {
			int ko = 0;
			for(int i = 0; i < nbreOfOp; i++) {
				if(opteam[i] == nullptr) {
					ko++;
				} else if(opteam[i]->getHP() <= 0) {
					ko++;
				}
			}
			return (ko == 6);
		}

		int OpTeam::getSize() const {
			return nbreOfOp;
		}

	} // namespace Model
} // namespace OpMon
