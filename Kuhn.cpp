#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
using namespace std;

const int NUM_ACTIONS = 2;

class Node {
public:
	string info_set{};
	vector<double> regret_sum;
	vector<double> strategy_sum;

	vector<double> GetStrategy(double realization_weight) {
		double normalizing_sum{};
		vector<double> strategy(NUM_ACTIONS);
		for (int i{}; i < NUM_ACTIONS; ++i) {
			strategy[i] = regret_sum[i] > 0.0001 ? regret_sum[i] : 0;
			normalizing_sum += strategy[i];
		}
		for (int i{}; i < NUM_ACTIONS; ++i) {
			if (normalizing_sum > 0) {
				strategy[i] /= normalizing_sum;
			} else {
				strategy[i] = 1.0 / NUM_ACTIONS;
			}
			strategy_sum[i] += realization_weight * strategy[i];
		}
		return strategy;
	}

vector<double> GetAverageStrategy() {
	vector<double> average_strategy(NUM_ACTIONS, 0.0);
	double normalizing_sum{};
	for (int i{}; i < NUM_ACTIONS; i++) {
		normalizing_sum += strategy_sum[i];
	}
	for (int i{}; i < NUM_ACTIONS; i++) {
		if (normalizing_sum > 0) {
			average_strategy[i] = strategy_sum[i] / normalizing_sum;
		} else {
			// initial strategy profile
			average_strategy[i] = 1.0 / NUM_ACTIONS;
		}
	}
	return average_strategy;
}

	Node() {
		// initialize cumulative regret and cumulative strategy tables
		regret_sum.assign(NUM_ACTIONS, 0.0);
		strategy_sum.assign(NUM_ACTIONS, 0.0);
	}
};

// information set I
map<string, Node*> node_map{};
vector<int> cards{1,2,3};

double cfr(string h, double p0, double p1) {
	// determine player and observe history
	int plays = h.size();
	int player = plays % 2;
	int opp = 1 - player;

	// return payoff for terminal states, determine if h is terminal
	if (plays > 1) {
		bool terminalPass = h[plays - 1] == 'p';
		bool doubleBet = h.substr(plays-2, plays) == "bb";
		bool isPlayerCardHigher = cards[player] > cards[opp];
		if (terminalPass) {
			if (h == "pp") {
				return isPlayerCardHigher ? 1 : -1;
			} else {
				return 1;
			}
		} else if (doubleBet) {
			return isPlayerCardHigher ? 2 : -2;
		}
	}
	string info_set = to_string(cards[player]) + h;

	// Get infoset node or create if nonexistent.
	Node* node;
	if (node_map.find(info_set) == node_map.end()) {
		node = new Node();
		node->info_set = info_set;
		node_map[info_set] = node;
	} else {
		node = node_map[info_set];
	}

	// For each action, recursively call cfr w/ additional history and probability
	vector<double> strategy = node->GetStrategy(player == 0 ? p0 : p1);
	vector<double> util(NUM_ACTIONS, 0.0);
	double nodeUtil{};
	// We are at chance node
	for (int i = 0; i < NUM_ACTIONS; ++i) {
		string nextHistory = h + (i == 0 ? "p" : "b");
		util[i] = (player == 0)
			? -1 * cfr(nextHistory, p0 * strategy[i], p1)
			: -1 * cfr(nextHistory, p0, p1 * strategy[i]);
		nodeUtil += strategy[i] * util[i];
	}

	// for each action, compute and accumulate cfr regret
	for (int i = 0; i < NUM_ACTIONS; ++i) {
		double regret = util[i] - nodeUtil;
		node->regret_sum[i] += (player == 0 ? p1 : p0) * regret;
	}
	return nodeUtil;
}

void Train(int iterations) {
	double util = 0.0;
	for (int i = 0; i < iterations; i++) {
		// Fisher-Yates shuffle
		for (int c1 = 2; c1 > 0; c1--) {
			int c2 = (rand() % (c1 + 1));
			swap(cards[c1], cards[c2]);
		}

		util += cfr("", 1, 1);
		if (i % 10000 == 0) {
			cout << "EV player 1: " << util / iterations << "\n";
			for (auto& n : node_map) {
				Node* node = n.second;
				vector<double> a_s = node->GetAverageStrategy();
				cout << n.first << " " << a_s[0] << " " << a_s[1] << "\n";
			}
			cout <<"\n";
		}
	}
}

int main() {
	srand((unsigned) time(0));
	Train(100000);
}
