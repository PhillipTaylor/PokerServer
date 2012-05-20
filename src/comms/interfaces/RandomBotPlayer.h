/*
 * RandomBotPlayer.h
 *
 * RandomBotPlayer is a concrete player, able to take actions in a game
 * of poker as if it were a real player. RandomBot player ignores the
 * cards dealt to it, and chooses what to do based (bet,call,fold) on the results of calling
 * rand() making it totally random. The primary purpose of this class is that it's used
 * during the test suite to allow the developer to replay logic is a wide number of scenarios
 * quickly. The randomness of this class may very when I wanted to simulate longer games but
 * the output is generally unpredictable.
 */

#ifndef RANDOMBOTPLAYER_H_
#define RANDOMBOTPLAYER_H_

#include "core/AbstractPlayer.h"
#include "core/Money.h"

using namespace GameEngineCore;

namespace ConcretePlayers {

class RandomBotPlayer: public AbstractPlayer {
public:
	RandomBotPlayer();

	bool IsPlaying();
	std::string GetName();
	Money GetTotalBalance();
	void SetTotalBalance(Money new_value);

	void DealerAnnounce(std::string dealer);
	void SmallBlindAnnounce(std::string payer, Money amount);
	void BigBlindAnnounce(std::string payer, Money amount);
	void OpponentMoneyUpdate(std::string player, Money pot, Money bank);
	void OpponentCardAnnounce(std::string player, const Hand& players_hand);
	void WinnerAnnounce(std::string player, Money winnings);

	void ChatMessage(std::string sender, std::string message);
	void CardDealt(const Hand& hand, const Card& new_card);
	GameChoice MakeChoice(Money minimum_bid);

	~RandomBotPlayer() {}

private:
	Money m_total_balance;
	std::string m_name;


};

} /* namespace ConcretePlayers */
#endif /* RANDOMBOTPLAYER_H_ */
