#define ASIO_NO_WINDOWS_H
#include "crow_all.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "json.hpp"
#include <cctype>
#include <sstream>
#include <cstdlib>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

using namespace crow;
using namespace std;
using namespace websocket;

#define ll long long
bool contains(vector<int>& a, int b) {
    if (a.size() == 0) return false;
    return find(a.begin(), a.end(), b) != a.end();
} 

bool contains(vector<string>& a, string b) {
    if (a.size() == 0) return false;
    return find(a.begin(), a.end(), b) != a.end();
} 

bool isNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

struct CORS {
    struct context {};

    void before_handle(request& req, response& res, context&) {
        // Handle preflight OPTIONS requests globally
        if (req.method == HTTPMethod::Options) {
            res.code = 204;
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, X-Requested-With");
            res.end();
        }
    }

    void after_handle(request& req, response& res, context&) {
        // For all responses (not just OPTIONS), add CORS headers
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type, X-Requested-With");
    }
};

class Ability {
    public:
    string name = "Ability";
	string time = "Night";
	int charges = -1;
	int cooldown = 0;
	int cost = 0;
	vector<string> target = {"alive"};
	int func = 0;
	vector<string> params = {};
    bool visiting = true;
    bool ghost = false;
    vector<int> currTarg = {-1};
	int id = 0;
};
bool operator==(const Ability& a, const Ability& b) {
    return a.id == b.id;
}

int rand(int lower, int upper) {
    --upper;
    if (upper <= lower) return lower;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(lower, upper);
    return dist(gen);
}

class Role {
    public:
    string name = "None";
    string alignment = "None";
    string type = "None";
    bool unique = true;
    vector<Ability> abs = vector<Ability>{};
    vector<Ability> parliament = vector<Ability>{};
    Role(string name = "None", string alignment = "None", string type = "None", vector<Ability> abs = vector<Ability>{}, vector<Ability> par = vector<Ability>{}) {
        this->name = name;
        this->alignment = alignment;
        this->type = type;
        this->abs = abs;
        this->parliament = par;
    }
};

template <typename T>
vector<T> flatten(vector<vector<T>> bucket) {
    vector<T> ret;
    for (vector<T>& rs : bucket) {
        for (T& r : rs) {
            ret.push_back(r);
        }
    }
    return ret;
};

map<string, vector<string>> buckets {
    {
        "Town Power", 
        vector<string>{ 
            "Gunner",
            "Jailor",
            "Transporter",
            "Marshal"
        }
    }, {
        "Town Inspection", 
        vector<string>{ 
            "Sheriff",
            "Seer",
            "Psychic",
            "Bard"
        }
    }, {
        "Town Scout", 
        vector<string>{ 
            "Coroner",
            "Hacker",
            "Accountant",
            "Spy"
        }
    }, {
        "Town Investigative", 
        vector<string>{ 
            "Investigator",
            "Tracker",
            "Auditor",
            "Gambler"
        }
    }, {
        "Town Protective", 
        vector<string>{ 
            "Doctor",
            "Watcher",
            "Locksmith",
            "Trapper"
        }
    }, {
        "Town Killing", 
        vector<string>{ 
            "Vigilante",
            "Flagger",
            "Agent",
            "Cupid"
        }
    }, {
        "Town Economic", 
        vector<string>{ 
            "Merchant",
            "Pickpocket",
            "Trainer",
            "Investor"
        }
    }, {
        "Town Support", 
        vector<string>{ 
            "Bartender",
            "Deputy",
            "Scholar",
            "Trickster"
        }
    }, {
        "Mafia Power", 
        vector<string>{ 
            "Godfather",
            "Propagandist",
            "Mastermind",
            "Bomber"
        }
    }, {
        "Mafia Killing", 
        vector<string>{ 
            "Jackal",
            "Barbarian",
            "Conspirator",
            "Kidnapper"
        }
    }, {
        "Mafia Deception", 
        vector<string>{ 
            "Framer",
            "Disguiser",
            "Phasmologist",
            "Rigger"
        }
    }, {
        "Mafia Economic", 
        vector<string>{ 
            "Forger",
            "Robber",
            "Ambusher",
            "Launderer"
        }
    }, {
        "Mafia Support", 
        vector<string>{ 
            "Sniper",
            "Bootlegger",
            "Consigliere",
            "Insider"
        }
    }
};

void fillBuckets() {
    buckets["Town Enforcer"] = flatten(vector<vector<string>>{ 
        buckets["Town Power"],
        buckets["Town Inspection"]
    });
    buckets["Common Town"] = flatten(vector<vector<string>>{ 
        buckets["Town Scout"],
        buckets["Town Investigative"],
        buckets["Town Protective"],
        buckets["Town Killing"],
        buckets["Town Economic"],
        buckets["Town Support"],
    });
    buckets["Salient Mafia"] = flatten(vector<vector<string>>{ 
        buckets["Mafia Power"],
        buckets["Mafia Killing"],
    });
    buckets["Common Mafia"] = flatten(vector<vector<string>>{ 
        buckets["Mafia Deception"],
        buckets["Mafia Economic"],
        buckets["Mafia Support"],
    });
    buckets["Mayor"] = flatten(vector<vector<string>>{ 
        buckets["Common Town"],
        buckets["Common Mafia"],
        // buckets["Neutral Killing"],
        // buckets["Neutral Support"],
        // buckets["Neutral Outcast"],
    });
}

vector<string> roleNames = {
        "Gunner",
        "Jailor",
        "Transporter",
        "Marshal",

        "Sheriff",
        "Seer",
        "Psychic",
        "Bard",

        "Coroner",
        "Hacker",
        "Accountant",
        "Spy",

        "Investigator",
        "Tracker",
        "Auditor",
        "Gambler",

        "Doctor",
        "Watcher",
        "Locksmith",
        "Trapper",

        "Vigilante",
        "Flagger",
        "Agent",
        "Cupid",

        "Merchant",
        "Pickpocket",
        "Trainer",
        "Investor",

        "Bartender",
        "Deputy",
        "Scholar",
        "Trickster",

        "Godfather",
        "Propagandist",
        "Mastermind",
        "Bomber",

        "Jackal",
        "Barbarian",
        "Conspirator",
        "Kidnapper",

        "Forger",
        "Robber",
        "Ambusher",
        "Launderer",

        "Framer",
        "Phasmologist",
        "Disguiser",
        "Rigger",

        "Bootlegger",
        "Consigliere",
        "Insider",
        "Sniper",

        "Serial Killer",
        "Reaper",
        "Lost Soul",
        "Fallen Angel",

        "Amnesiac",
        "Hex Master",
        "Tactician",
        "Ritualist",

        "Carnifex",
        "Scammer",
        "Beguiler",
        "Thief",

        "Entrepeneur",
        "Anarchist",
        "Corrupt",
        "Beamer",

        "Executioner",
        "Inquisitor",
        "Devout",
        "Politician",
};

class Rolelist {
    public:
    vector<vector<string>> rolelist;
    vector<string> whitelist;
    vector<string> generate (int count) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, 999999);
        vector<string> ret;
        int i = 0;
        for (vector<string> r : rolelist) {
            vector<string> used;
            for (string& e : r) {
                auto it = buckets.find(e);
                if (it == buckets.end()) {
                    used.push_back(e);
                } else {
                    for (string n : (*it).second) {
                        if (contains(whitelist, n)) 
                        used.push_back(n);
                    }
                }
            }
            
            if (!used.empty()) {
                string chosen = used[dist(gen) % used.size()];
                ret.push_back(chosen);
            }
            ++i;
            if (i >= count) break;
        }
        return ret;
    }
};

Ability killAb = Ability {
    name: "Kill",
    time: "Night*",
    func: 1,
};

map<string, Role> roles {
    {
        "Gunner", Role("Gunner", "Town", "Power",
        vector<Ability> {
            Ability {
                name: "Shoot",
                time: "Night*",
                charges: 1,
                func: 1,
            },
            Ability {
                name: "Buy",
                cost: 4,
                target: {"check"},
                func: 2,
                params: vector<string>{"0", "-1", "1", "1"},
                visiting: false,
            },
        }, vector<Ability>{
            Ability {
                name: "Negotiate",
                time: "Passive",
                target: {},
                func: 3,
                params: vector<string>{"0", "-2", "-1", "1"}
            },
        })
    },  
    {"Sheriff", Role("Sheriff", "Town", "Inspection",
        vector<Ability> {
        Ability {
            name: "Interrogate",
            cost: 1,
            func: 5,
        },
    }, vector<Ability>{
        Ability {
            name: "Expert Investigation",
            charges: 2,
            cost: 4,
            func: 6,
        },
    })},
    {"Coroner", Role("Coroner", "Town", "Scout",
        vector<Ability> {
        Ability {
            name: "Autopsy",
            cost: 1,
            func: 11,
            visiting: false,
        },
    }, vector<Ability>{
        Ability {
            name: "Autopsy",
            charges: 2,
            cost: 3,
            func: 12,
            visiting: false,
        },
    })},
    {"Investigator", Role("Investigator", "Town", "Investigative",
        vector<Ability> {
        Ability {
            name: "Investigate",
            cost: 1,
            func: 13,
        },
    }, vector<Ability>{
        Ability {
            name: "Deduce",
            charges: 2,
            cost: 3,
            func: 30,
        },
    })},
    {"Gambler", Role("Gambler", "Town", "Investigative",
        vector<Ability> {
        Ability {
            name: "Bonus",
            time: "Initial",
            target: {},
            func: 16,
            params: {"2"},
            visiting: false,
        },
        Ability {
            name: "Gamble",
            cost: 3,
            func: 31,
        },
    }, vector<Ability>{
        Ability {
            name: "Gamble wisely",
            cost: 4,
            target: {"alive", "alive"},
            func: 31,
        },
    })},
    {"Doctor", Role("Doctor", "Town", "Protective",
        vector<Ability> {
        Ability {
            name: "Heal",
            cost: 2,
            func: 14,
        },
        Ability {
            name: "Reward",
            time: "OnKillBlocked",
            cost: 0,
            func: 15,
            visiting: false,
        },
        Ability {
            name: "Bonus",
            time: "Initial",
            target: {},
            func: 16,
            params: {"2"},
            visiting: false,
        },
    }, vector<Ability>{
        Ability {
            name: "Inocculate",
            charges: 1,
            cost: 5,
            func: 32,
        },
    })},
    {"Merchant", Role("Merchant", "Town", "Economic",
        vector<Ability> {
        Ability {
            name: "Yay",
            time: "Initial",
            target: {},
            func: 17,
            visiting: false,
        },
        Ability {
            name: "Invest",
            cost: 3,
            target: {"check"},
            func: 17,
            visiting: false,
        },
    }, vector<Ability>{
        Ability {
            name: "Train",
            charges: 2,
            cost: 3,
            target: {"alive"},
            func: 33,
        },
    })},
    {"Vigilante", Role("Vigilante", "Town", "Killing",
        vector<Ability> {
        Ability {
            name: "Shoot",
            cost: 5,
            func: 18,
        },
    }, vector<Ability>{
        Ability {
            name: "Reveal",
            time: "Night",
            charges: 1,
            func: 29,
        },
    })},
    {"Deputy", Role("Deputy", "Town", "Killing",
        vector<Ability> {
        Ability {
            name: "Ready",
            time: "OnKill",
            func: 19,
            visiting: false,
        },
    }, vector<Ability>{
        Ability {
            name: "Informed",
            time: "OnElect",
            func: 34,
            visiting: false,
        }
    })},
    {"Godfather", Role("Godfather", "Mafia", "Power",
        vector<Ability> {
        Ability {
            name: "Rich",
            time: "Initial",
            func: 35,
            visiting: false,
        },
    }, vector<Ability>{
        Ability {
            name: "Win",
            time: "OnElect",
            func: 36,
            params: {"Mafia", "The Godfather has been elected as Mayor! The Town has been colonized by the Mafia!"},
            visiting: false,
        }
    })},
    {"Jackal", Role("Jackal", "Mafia", "Killing",
        vector<Ability> {
            Ability {
                name: "Assassinate",
                time: "Night*",
                charges: 2,
                cost: 7,
                func: 1,
            },
        }, vector<Ability>{
            Ability {
                name: "Special Skills",
                time: "OnElect",
                func: 17,
            },
        })
    },
    {"Framer", Role("Framer", "Mafia", "Deception",
        vector<Ability> {
            Ability {
                name: "Frame",
                time: "Night",
                cost: 1,
                func: 20,
            },
        }, vector<Ability>{
            Ability {
                name: "FrameAll",
                time: "Night",
                cost: 1,
                func: 20,
            },
        })
    },
    {"Consigliere", Role("Consigliere", "Mafia", "Support",
        vector<Ability> {
            Ability {
                name: "Open Ears",
                time: "Passive",
                func: 26,
                visiting: false,
            },
            Ability {
                name: "Size Up",
                time: "Night",
                cost: 1,
                func: 21,
            },
            Ability {
                name: "Track",
                time: "Night",
                cost: 0,
                target: {},
                func: 22,
                visiting: false,
            },
        }, vector<Ability>{
            Ability {
                name: "Sweep",
                time: "Night",
                cost: 5,
                target: {"check"},
                func: 37,
                visiting: false,
            },
        })
    },
    {"Sniper", Role("Sniper", "Mafia", "Support",
        vector<Ability> {
            Ability {
                name: "Snipe",
                time: "Night*",
                charges: 2,
                cost: 3,
                target: {"check"},
                func: 23,
                visiting: false,
            },
        }, vector<Ability>{
            Ability {
                name: "Fine",
                time: "Night",
                target: {"alive"},
                func: 38,
                params: {"5"},
            },
        })
    },
    {"Serial Killer", Role("Serial Killer", "Killer", "Killing",
        vector<Ability> {
            killAb,
            Ability {
                name: "Asassinate",
                time: "Night*",
                cost: 5,
                target: {"check"},
                func: 23,
                visiting: false,
            },
            Ability {
                name: "Bloodlust",
                time: "Night*",
                cost: 8,
                func: 23,
            },
            Ability {
                name: "Immune",
                time: "Initial",
                func: 25,
            },
        }, vector<Ability>{
            Ability {
                name: "Rampage",
                time: "Night*",
                cost: 6,
                func: 39,
            },
        })
    },
    {"Amnesiac", Role("Amnesiac", "Killer", "Support",
        vector<Ability> {
            Ability {
                name: "Open Ears",
                time: "Passive",
                func: 26,
            },
            Ability {
                name: "Contempt",
                time: "OnKill",
                func: 27,
            },
            Ability {
                name: "Size Up",
                time: "Night",
                cost: 1,
                func: 21,
            },
            Ability {
                name: "Track",
                time: "Night",
                cost: 0,
                target: {},
                func: 22,
            },
        }, vector<Ability>{
            
        })
    },
    {"Townie", Role("Townie", "Town", "Special")},
    {"Mafioso", Role("Mafioso", "Mafia", "Special")},
};

bool operator==(const Role& a, const Role& b) {
    return a.name == b.name &&
    a.abs == b.abs &&
    a.parliament == b.parliament;
}

vector<Role> BtoW(vector<Role> blacklist) {
    vector<Role> ret;
    for (auto& [name, r] : roles) {
        if (find(blacklist.begin(), blacklist.end(), r) == blacklist.end()) {
            ret.push_back(r);
        }
    }
    return ret;
}

class Player {
    public:
    bool alive = true;
    int coins = 1;
    int income = 1;
    int frameDefect = -1;
    int localId = 0;
    string username;
    string alignment = "None";
    Role role = Role();
    string id = "error_id_ping_cyan_fix_now";
    int vote = -1;
    int factionalVote = -1;
    int tag = -1;
    int visit = -1;
    int defense = 0;
    bool visitLocked = false;
    vector<int> killers = {};
    vector<string> effects = vector<string>{};
    vector<Ability> abilities = vector<Ability>{};
    vector<pair<int, string>> chat = vector<pair<int, string>>{};
    Player() : Player("default") {}
    Player (string username) {
        this->username = username;
        this->id = username;
    }
};

bool contains(vector<Role>& a, Role b) {
    if (a.size() == 0) return false;
    return find(a.begin(), a.end(), b) != a.end();
} 

bool contains(vector<string>& a, Role b) {
    if (a.size() == 0) return false;
    return find(a.begin(), a.end(), b.name) != a.end();
} 

bool operator==(const Player& a, const Player& b) {
    return a.id == b.id;
}
class GameState {
    public:
    string mode;
    string lobbyCode;
    int mayor;
    int starting;
    vector<Player> players;
    string phase;
    int dayNum;
    int condemns;
    float timer;
    bool started = false;
    Rolelist rolelist;
    GameState() : GameState("", "") {}
    GameState (string code, string mode) {
        this->lobbyCode = code;
        this->mayor = 0;
        this->players = vector<Player>{};
        this->phase = "Pregame";
        this->dayNum = 0;
        this->mode = mode;
        this->condemns = 0;
        this->timer = 0;
    }
};



bool operator==(const GameState& a, const GameState& b) {
    return a.lobbyCode == b.lobbyCode;
}

/*
RETURN CODES
0: success
1: target immune
2: they're already dead
*/


map<string, Rolelist> preset_rolelists {
    {
        "Mafia's%20Invitation", Rolelist {
        rolelist: vector<vector<string>> {
            {"Sheriff"}, {"Deputy"}, {"Common Town"}, {"Common Mafia"},
            {"Common Mafia"}, {"Common Town"}, {"Coroner", "Gambler"}, {"Jackal"},
            {"Gunner"}, {"Serial Killer"}, {"Doctor", "Vigilante"}, {"Merchant"},
            {"Amnesiac"}, {"Common Town"}, {"Common Mafia"}, {"Common Town"},
        },
        whitelist: {"Sheriff", "Gunner", "Coroner", "Gambler",
            "Doctor", "Vigilante", "Merchant", "Deputy", "Godfather",
            "Jackal", "Consigliere", "Framer", "Sniper",
            "Serial Killer", "Amnesiac"}
    }},
};

class lobbyState {
    public:
    map<string, string> tokenRef;           // token → player ID
    map<string, int> playerIds;             // player ID → local ID
    map<string, string> lobbies;            // player ID → lobby code
    map<string, connection*> conns;         // player ID → connection pointer
    map<string, GameState> gamestates;      // lobby code → gamestate obj
};

void updateChat(lobbyState& s, Player& p) {
    string res = "";
    nlohmann::json j;
    nlohmann::json chatArray;
    cout << "Sending system message to: " << p.username << "\n";
    for (auto& [id, msg] : p.chat) {
        cout << "Entry -> senderId: " << id << ", msg: " << msg << "\n";
    }
    for (auto& [senderId, msg] : p.chat) {
        nlohmann::json t;
        t["playerId"] = senderId + 1;
        if (senderId == -1) {
            t["username"] = "frontend glitch tell cyan to fix";
        } else {
            t["username"] = s.gamestates[s.lobbies[p.id]].players[senderId].username;
        }
        t["message"] = msg;
        chatArray.push_back(t);
    }
    j["type"] = "Chat";
    j["content"] = chatArray;

    if (s.conns.count(p.username) == 0 || s.conns[p.username] == nullptr) {
        return;
    }
    res = j.dump();
    if (s.conns.find(p.username) != s.conns.end()) s.conns[p.username]->send_text(res);
}

void send(lobbyState& s, Player& p, string message) {
    p.chat.push_back({-1, message});
    updateChat(s, p);
}
Player server = Player("Server");

int ability(Player& user, Ability& ab, GameState& gs, lobbyState& s);

void killTrigger(lobbyState& ls, GameState& gs, Player& victim, Player& killer = server, bool succeed = false) {
    for (Player& p : gs.players) {
        for (Ability& a : p.abilities) {
            if (a.time == (succeed ? "OnKill" : "OnKillBlocked")) {
                a.currTarg = {victim.localId};
                ability(p, a, gs, ls);
            }
        }
    }
}

void endGame(lobbyState& ls, GameState& gs, string winner, string message) {
    for (Player& p : gs.players) {
        send(ls, p, "[yellow]" + message);
    }
    gs.timer = 999999999;
    gs.phase = "Game Ended";
}

string checkEnd(GameState& gs) {
    map<string, int> alive; 
    for (Player &p : gs.players) {
        if (p.alive) {
            ++alive[p.alignment];
        }
    }
    string res = "draw";
    for (pair<string, int> psi : alive) {
        if (psi.first == "Solo" || psi.first == "Outcast") continue;
        else if (res == "draw") {
            res = psi.first;
        } else {
            res = "";
        }
    }
    return res;
}

int kill(lobbyState& ls, GameState& gs, Player& victim, int str = 1, Player& killer = server) {
    server.localId = -1;
    cout << "[KILL] Trying to kill " << victim.username << ", alive = " << victim.alive << endl;
    bool scumshield = false;
    if (victim.defense >= 10) {
        scumshield = true;
    }
    victim.defense %= 10;
    if (!victim.alive) {
        if (killer.localId != -1) victim.killers.push_back(killer.localId);
        return 2;
    }
    if (str <= victim.defense) {
        killTrigger(ls, gs, victim, killer);
        if (scumshield) victim.defense = 0;
        return 1;
    }
    killTrigger(ls, gs, victim, killer, true);
    victim.killers.push_back(killer.localId);
    nlohmann::json j;
    j["type"] = "Kill";
    j["victim"] = victim.username;
    for (Player& p : gs.players) {
        send(ls, p, "[red]" + victim.username + " has died.");
        if (ls.conns.find(p.id) != ls.conns.end()) {
            ls.conns[p.id]->send_text(j.dump());
        }
    }
    victim.alive = false;
    string winner = checkEnd(gs);
    if (winner == "") return 0;
    endGame(ls, gs, winner, "The " + winner + " has won, for none of their enemies remain!");
    return 0;
}

/*
FUNCTIONS:
1 - kill(target)
2 - change ability charges (target, relativeId, value, increment?)
3 - change ability price (target, relativeId, value, increment?)
4 - change ability cooldown (target, relativeId, value, increment?)
5 - sheriff check
6 - rolecop
*/

vector<Ability> globalAbs = {
    Ability {
        name: "Give Coins",
        target: {"alive", "number"},
        func: 7,
        ghost: true,
    },
    Ability {
        name: "Vote",
        time: "Voting",
        func: 8,
    },
    Ability {
        name: "Vote",
        time: "Election",
        func: 8,
    },
    Ability {
        name: "Passive Income",
        target: vector<string>{},
        func: 9,
    }
};
Ability& FindAb(GameState& gs, int id) {
    for (Player &p : gs.players) {
        for (Ability &a : p.abilities) {
            if (a.id == id) return a;
        }
    }
    static Ability invalid = Ability();
    invalid.id = -1;
    return invalid;
}

string generateToken() {
    string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()-_=+[{}]|/.,<>;:";
    random_device rd; // non-deterministic random device
    mt19937 gen(rd()); // seed mt19937 from rd
    uniform_int_distribution<> dist(0, charset.size() - 1);

    string token;
    for (size_t i = 0; i < 32; ++i) {
        token += charset[dist(gen)];
    }
    return token;
}

vector<int> prio = {
    2, 3, 4, 23, 39, //ability modifiers
    24, 25, //general modifiers
    14, //protect
    20, //frame
    1, 18, //MURDER
    17, //income
    7, 9, 15, 16, 37, 38, //money
    8, 10, //attribute mod
    5, 6, 11, 12, 13, 21, 29, 30, //info
    22, //tracking
    28, //money tracking
};

Ability& findTargetAb(Ability& ab, GameState& gs) {
    int re = (ab.params[0] == "-") ? ab.currTarg[0] : ab.id + stoi(ab.params[0]);
    return FindAb(gs, re);
}

string checkRole(Player& p, GameState& gs) {
    string ret = p.role.name;
    for (int i = 0; i < p.effects.size(); ++i) {
        if (p.effects[i] == "ShowRole") ret = p.effects[i+1];
    }
    if (gs.mayor == p.localId) ret = "Mayor";
    return ret;
}

string checkAli(Player& p, GameState& gs) {
    string ret = p.alignment;
    if (p.frameDefect != -1) {
        ret = gs.players[p.frameDefect].alignment;
    }
    for (string effect : p.effects) {
        if (effect == "ShowInno") ret = "Town";
    }
    if (gs.mayor == p.localId) ret = "Mayor";
    return ret;
}

void showRole(lobbyState& ls, GameState& gs, Player& p, Player& targ, string r) {
    nlohmann::json j;
    j["type"] = "RoleReveal";
    j["player"] = targ.username;
    j["role"] = r;
    if (ls.conns.find(p.id) != ls.conns.end()) {
        ls.conns[p.id]->send_text(j.dump());
    }
    
}

int giveCoins(lobbyState& ls, GameState& gs, Player& p, int amount) {
    p.coins += amount;
    if (p.coins < 0) {
        amount -= p.coins;
        p.coins = 0;
    }
    nlohmann::json j;
    j["type"] = "UpdateCoins";
    j["coins"] = p.coins;
    if (ls.conns.find(p.id) != ls.conns.end()) {
        ls.conns[p.id]->send_text(j.dump());
    }
    return amount;
} 
/*
RETURN CODES:
0: success
1: disrupted
2: no charges
3: too broke
4: not used
5: on cooldown
6: invalid ability
7: malformed ability
8: user is DEAD (and its not a ghost ability)
*/
int ability(Player& user, Ability& ab, GameState& gs, lobbyState& s) {
    // cout << "[ability] Called: " << ab.name << " (func=" << ab.func << "), user=" << user.username << "\n";
    if (ab.currTarg.size() > 0) {
        bool ow = false;
        for(int e : ab.currTarg) {
            if (e <= 0) ow = true;
        }
        if (ow) return 4;
        if (ab.cooldown > 0) return 5;
        if (ab.cost > user.coins) return 3;
        if (ab.charges == 0) return 2;
        if (!user.alive && !ab.ghost) return 8;
    }
    switch(ab.func) {
        case 1: { //Kill
            Player& p = gs.players[ab.currTarg[0] - 1];
            int ret = kill(s, gs, p, 1 + (ab.params.size() > 0), user);
            if (contains(ab.params, "Rampage")) {
                for (Player& ramp : gs.players) {
                    if (ramp.visit == p.localId) kill(s, gs, ramp, 1 + (ab.params.size() > 0), user);
                }
            }
            user.visitLocked = true;
            if (ret != 1 && user.role.name == "Serial Killer") {
                int coins = giveCoins(s, gs, p, -p.coins);
                giveCoins(s, gs, user, coins);
            }
        }
        break;
        case 2: { //Recharge ability
            Ability& a = findTargetAb(ab, gs);
            if (a.id == -1) return 7;
            if (ab.params[2] == "1") {
                a.charges += stoi(ab.params[1]);
            } else {
                a.charges = stoi(ab.params[1]);
            }
        }
        break;
        case 3: { //Change ability price
            Ability& a = findTargetAb(ab, gs);
            if (a.id == -1) return 7;
            if (ab.params[2] == "1") {
                a.cost += stoi(ab.params[1]);
            } else {
                a.cost = stoi(ab.params[1]);
            }
        }
        break;
        case 4: { //Change ability cooldown
            Ability& a = findTargetAb(ab, gs);
            if (a.id == -1) return 7;
            if (ab.params[2] == "1") {
                a.cooldown += stoi(ab.params[1]);
            } else {
                a.cooldown = stoi(ab.params[1]);
            }
        }
        break;
        case 5: {
            //Sheriff check
            Player& p = gs.players[ab.currTarg[0] - 1];
            string ali = checkAli(p, gs);
            if (ali == "Mayor") {
                send(s, user, "[purple]Your target is the Mayor. You cannot determine the Mayor's alignment.</span>");
            } if (ali == "Town" || ali == "Killer") { 
                send(s, user, "[green]Your target seems innocent.</span>");
            } else {
                send(s, user, "[red]Your target seems EVIL!</span>");
            }
        }
        break;
        case 6: { //Rolecop
            Player& p = gs.players[ab.currTarg[0] - 1];
            string role = checkRole(p, gs);
            send(s, user, "Your target is a " + role + "!");
            showRole(s, gs, user, p, role);
        }
        break;
        case 7: { //Give Coins
            Player& targ = gs.players[ab.currTarg[0] - 1];
            if (user.coins < ab.currTarg[1]) {
                return 3;
            } else {
                int given = -giveCoins(s, gs, user, -ab.currTarg[1]);
                giveCoins(s, gs, targ, given);
            }
        }
        break;
        case 8: { //Vote
            user.vote = ab.currTarg[0] - 1;
        }
        break;
        case 9: { //Passive Income
            giveCoins(s, gs, user, user.income);
        }
        break;
        case 10: { //Soldier Vote
            user.factionalVote = ab.currTarg[0] - 1;
        }
        break;
        case 11: { //Coroner
            Player& p = gs.players[ab.currTarg[0] - 1];
            string role = checkRole(p, gs);
            send(s, user, "Your target is a " + role + "!");
            showRole(s, gs, user, p, role);
            vector<int> killers = p.killers;
            for (int i : killers) {
                send(s, user, "They were killed by a " + gs.players[i].role.name + "!");
            }
        }
        break;
        case 12: { //Seer/Coroner Parliament
            Player& p = gs.players[ab.currTarg[0] - 1];
            vector<int> killers = p.killers;
            vector<int> alive;
            for (int i = 0; i < gs.players.size(); ++i) {
                if (i == user.localId) continue;
                if (!gs.players[i].alive) continue;
                alive.push_back(i);
            }
            for (int i : killers) {
                if (gs.players[i].alignment == "Killers") {
                    send(s, user, "Your target was killed by a " + gs.players[i].role.name + ", so you couldn't figure out any suspects.");
                } else {
                    int ind = rand(0, alive.size());
                    if (i == alive[ind]) {
                        ind += rand(0, alive.size() - 1);
                        ind %= alive.size();
                    }
                    int redHerring = alive[ind];
                    if (rand(0, 2) == 0) {
                        send(s, user, "Your target was killed by " + to_string(i) + " " + gs.players[i].username + " or " + to_string(redHerring) + " " + gs.players[redHerring].username + " !");
                    } else {
                        send(s, user, "Your target was killed by " + to_string(redHerring) + " " + gs.players[redHerring].username + " or " + to_string(i) + " " + gs.players[i].username + " !");
                    }
                }
            }
        } 
        break;
        case 13: { //Investigator
            vector<vector<string>> results = {
                {"Power", "Killing", "Scout", "Solo"},
                {"Inspection", "Protective", "Deception", "Outcast", "Special"},
                {"Investigative", "Economic", "Support"}
            };
            Player& p = gs.players[ab.currTarg[0] - 1];
            string ali = p.role.type;
            if (p.role.name == "Grob") {
                send(s, user, "Your target is Grob.");
            } else {
                for (vector<string>& e : results) {
                    if (contains(e, ali)) {
                        string ret = "Your target's role type could be ";
                        int i = e.size();
                        for (string s : e) {
                            ret += s;
                            if (i == 2) {
                                ret += ", or ";
                            } else if (i == 1) {
                                ret += "!";
                            } else {
                                ret += ", ";
                            }
                            --i;
                        }
                        send(s, user, ret);
                    }
                }
            }
        }
        break;
        case 14: { //Doctor protection
            Player& p = gs.players[ab.currTarg[0] - 1];
            p.defense = max(p.defense, 1);
            user.tag = ab.currTarg[0] - 1;
        }
        break;
        case 15: { //Doctor bonus
            cout<<"Reward to "<<ab.currTarg[0]<<" => "<<user.tag<<"\n";
            if (ab.currTarg[0] == user.tag) {
                giveCoins(s, gs, user, 3);
            }
        }
        break;
        case 16: { //Starting bonus
            giveCoins(s, gs, user, stoi(ab.params[0]));
        }
        break;
        case 17: { //Self increase income
            ++user.income;
        }
        break;
        case 18: { //Vigilante shoot
            Player& p = gs.players[ab.currTarg[0] - 1];
            if (p.alignment != "Town") kill(s, gs, gs.players[ab.currTarg[0] - 1], false, user);
            if (p.alignment != "Town") user.visitLocked = true;
        }
        break;
        case 19: { //Deputy
            Player& p = gs.players[ab.currTarg[0]];
            if (p.alignment == "Town" && (p.role.type == "Inspection" || p.role.type == "Power")) {
                user.role = p.role;
                user.abilities = p.abilities;
                nlohmann::json j;
                j["type"] = "ShowRole";
                j["role"] = user.role.name;
                j["ingame"] = true;
                s.conns[user.id]->send_text(j.dump());
            }
        }
        break;
        case 20: { //Frame
            Player& p = gs.players[ab.currTarg[0] - 1];
            p.frameDefect = user.localId;
        }
        break;
        case 21: { //Consigliere/Amnesiac active
            Player& p = gs.players[ab.currTarg[0] - 1];
            for (int i = 0; i < 10; ++i) {
                p.effects.push_back("Sense" + to_string(user.localId));
            }
            string role = checkRole(p, gs);
            send(s, user, "Your target is a " + role + "!");
            showRole(s, gs, user, p, role);
        }
        break;
        case 22: { //Consigliere/Amnesiac passive
            for (Player& p : gs.players) {
                if (contains(p.effects, "Sense" + to_string(user.localId)) || contains(gs.players[p.visit].effects, "Sense" + to_string(user.localId))) {
                    send(s, user, to_string(p.localId + 1) + " " + p.username + " visited " + to_string(p.visit + 1) + " " + gs.players[p.visit].username + ".");
                }
            }
        }
        break;
        case 23: { //Sniper/SK assassinate
            for (Ability& a : user.abilities) {
                if (a.name == "Kill") {
                    a.params.push_back("lol");
                    a.visiting = false;
                }
            }
        }
        break;
        case 24: { //Effect
            Player& p = gs.players[ab.currTarg[0] - 1];
            if (ab.params.size() <= 0) return 7;
            else if (ab.params.size() == 1) ab.params.push_back("1");
            for (int i = 0; i < stoi(ab.params[1]); ++i) {
                p.effects.push_back(ab.params[0]);
            }
        }
        break;
        case 25: { //NK defense
            user.defense = 2;
        }
        break;
        case 26: { //Whisper reader

        }
        break;
        case 27: { //Amnesiac main
            Player& p = gs.players[ab.currTarg[0]];
            if (p.alignment == "Killer" && (p.role.type == "Killing")) {
                user.role = p.role;
                user.abilities = p.abilities;
                nlohmann::json j;
                j["type"] = "ShowRole";
                j["role"] = user.role.name;
                j["ingame"] = true;
                s.conns[user.id]->send_text(j.dump());
            }
        }
        break;
        case 28: { //Incomes
            for (Player& p : gs.players) {
                send(s, user, to_string(p.localId) + " " + p.username + "'s income is " + to_string(p.income) + "!");
            }
        }
        break;
        case 29: { //Mayor check
            Player& p = gs.players[ab.currTarg[0] - 1];
            string ali = checkAli(p, gs);
            send(s, user, "Your target's alignment is " + ali + "!");
        }
        break;
        case 30: { // Type cop
            Player& p = gs.players[ab.currTarg[0] - 1];
            string ali = p.role.type;
            if (p.role.name == "Grob") {
                send(s, user, "Your target is Grob.");
            } else {
                send(s, user, "Your target's role type is " + ali + ".");
            }
        }
        break;
        case 31: { // Gambler
            bool yay = false;
            for (int i : ab.currTarg) {
                Player& p = gs.players[i - 1];
                if (p.alignment != "Town") yay = true;
            }
            if (yay) giveCoins(s, gs, user, 2);
        }
        break;
        case 32: { //Doctor Inocculate
            Player& p = gs.players[ab.currTarg[0] - 1];
            p.defense = p.defense < 1 ? 11 : p.defense;
        }
        break;
        case 33: { //Increase other's income
            Player& p = gs.players[ab.currTarg[0] - 1];
            ++p.income;
        }
        break;
        case 34: { //Learn Town Enforcers
            for (Player& p : gs.players) {
                if (contains(buckets["Town Enforcer"], p.role)) {
                    send(s, user, "" + to_string(p.localId) + " " + p.username + " is a Town Enforcer.");
                }
            }
        }
        break;
        case 35: { //Godfather is rich
            for (Player& p : gs.players) {
                if (p.alignment == "Mafia") {
                    p.coins = 5;
                }
            }
        }
        break;
        case 36: { //Win
            string winner = ab.params[0], message = ab.params[1];
            endGame(s, gs, winner, message);
        }
        break;
        case 37: { //Consigliere Sweep
            for (Player& p : gs.players) {
                if (p.alignment == "Town" && p.alive) {
                    if (p.coins > 0) {
                        --p.coins;
                        ++user.coins;
                    }
                }
            }
        }
        break;
        case 38: { //Steal Coins
            Player& p = gs.players[ab.currTarg[0] - 1];
            int stolen = min(p.coins, stoi(ab.params[0]));
            giveCoins(s, gs, p, -stolen);
            giveCoins(s, gs, user, stolen);
        }
        break;
        case 39: { //SK Rampage
            for (Ability& a : user.abilities) {
                if (a.name == "Kill") {
                    a.params.push_back("lol");
                    a.visiting = false;
                }
            }
        }
        break;
        default: return 6;
    }
    if (!user.visitLocked && ab.visiting) {
        if (ab.currTarg.empty()) return 0;
        user.visit = ab.currTarg[0] - 1;
    }
    return 0;
}

string generateLobbyCode(map<string, GameState>& gamemodes) {
    cout << "generateLobbyCode() called, gamemodes.size(): " << gamemodes.size() << endl;
    string charset = "QWERTYUIOPASDFGHJKLZXCVBNM";
    string code = "AAAAAA";
    bool redo = true;
    while (redo)
    {
        random_device rd; // non-deterministic random device
        mt19937 gen(rd()); // seed mt19937 from rd
        uniform_int_distribution<> dist(0, charset.size() - 1);

        string ret;
        for (size_t i = 0; i < 6; ++i) {
            ret += charset[dist(gen)];
        }
        code = ret;
        redo = false;
        for (pair<string, GameState> e : gamemodes) {
            if (e.first == code) {
                redo = true;
                break;
            }
        }
    }
    return code;
}

const int MAX_PLAYERS = 16;

void updatePlayerList(lobbyState& ls, string code) {
    GameState& gs = ls.gamestates[code];
    nlohmann::json j;
    j["type"] = "PlayerList";
    nlohmann::json q;
    for (Player& tp : gs.players) {
        q.push_back(tp.username);
    }
    j["player-list"] = q;
    j["mayor"] = gs.players[gs.mayor].username;
    nlohmann::json list;
    list["type"] = "Rolelist";
    list["rolelist"] = gs.rolelist.rolelist;
    list["whitelist"] = gs.rolelist.whitelist;
    for (Player& p : gs.players) {
        if (ls.conns.find(p.id) != ls.conns.end()) {
            ls.conns[p.id]->send_text(j.dump());
            ls.conns[p.id]->send_text(list.dump());
        }
    }
}

string join(string mode, string playerId, lobbyState& s) {
    cout << "join() called with mode: " << mode << ", playerId: " << playerId << endl;
    string uppercase = "QWERTYUIOPASDFGHJKLZXCVBNM";
    bool customCode = true;
    for (char e : mode) {
        if (find(uppercase.begin(), uppercase.end(), e) == uppercase.end()) {
            customCode = false;
            break;
        }
    }
    cout << "customCode: " << customCode << endl;
    
    if (customCode) {
        GameState& gs = s.gamestates[mode];
        if (gs.players.size() == 0) {
            gs.mode = "Custom";
            gs.started = false;
        }
        if (gs.players.size() < MAX_PLAYERS && !gs.started) {
            s.playerIds[playerId] = gs.players.size();
            s.lobbies[playerId] = mode;
            gs.players.push_back(Player(playerId));
            updatePlayerList(s, mode);
            return "Join Successful!";
        } else return "Lobby was full or has already started.";
    } else {
        cout << "Handling normal mode path"<<endl;
        string code = "";
        for (const auto& e : s.gamestates) {
            cout << "Checking gamemode " << e.second.mode << " with mode " << e.first << endl;
            if (e.second.mode == mode) {
                if (s.gamestates[e.first].players.size() < MAX_PLAYERS && !e.second.started) {
                    code = e.first;
                    break;
                }
            }
        }
        
        if (code.size() == 0) {
            cout << "Generating new lobby code" << endl;
            code = generateLobbyCode(s.gamestates);
            s.gamestates[code] = GameState(code, mode);
            s.gamestates[code].started = false;
            if (preset_rolelists.find(mode) != preset_rolelists.end()) s.gamestates[code].rolelist = preset_rolelists[mode];
        }
        GameState& gs = s.gamestates[code];
        s.playerIds[playerId] = gs.players.size();
        s.lobbies[playerId] = code;
        gs.players.push_back(Player(playerId));
        cout << "Player joined lobby: " << code << endl;
        cout << "Players count after join: " << gs.players.size() << endl; //this prints correctly
        updatePlayerList(s, code);
        return "Join Successful!";
    }
    cout << "Default return reached" << endl;
    return "default";
}

void leave(string playerId, lobbyState& s) {
    GameState& gs = s.gamestates[s.lobbies[playerId]];
    if (s.playerIds[playerId] > 0) {
        int rem = s.playerIds[playerId];
        s.playerIds[playerId] = 0;
        string lobby = s.lobbies[playerId];
        auto p = find(gs.players.begin(), gs.players.end(), playerId);
        gs.players.erase(p);
        int c = 0;
        for (Player& pl : gs.players) {
            s.playerIds[pl.id] = c;
            gs.players[c].localId = c;
            ++c;
        }
        if (gs.players.size() == 0) s.gamestates.erase(gs.lobbyCode);
        s.lobbies[playerId] = "";
    }
}

void resetAbs(GameState& gs) {
    for (Player& p : gs.players) {
        for (Ability& a : p.abilities) {
            a.currTarg = vector<int>{};
            for (int i = 0; i < a.target.size(); ++i) {
                a.currTarg.push_back(-1);
            }
        }
    }
}

void procAbs(GameState& s, vector<string> times, lobbyState& ls) {
    for (int i : prio) {
        for (Player& p : s.players) {
            for (Ability& a : p.abilities) {
                if (contains(times, a.time) && a.func == i) {
                    if (ability(p, a, s, ls) == 0) {
                        giveCoins(ls, s, p, -a.cost);
                        --a.charges;
                    }
                    --a.cooldown;//cout<<"Processing: "<<a.name<<" => return code "<<ability(p, a, s, ls)<<'\n';
                    if (a.id < 0) {
                        p.abilities.erase(find(p.abilities.begin(), p.abilities.end(), a));
                    }
                }
            }
        }
    }
    resetAbs(s);
}
void endNight(GameState& s, lobbyState& ls) {
    if (s.dayNum == 0) {
        procAbs(s, {"Night"}, ls);
    } else {
        procAbs(s, {"Night", "Night*"}, ls);
    }
}

void showAbs(lobbyState& ls, GameState& gs, vector<string> phases) {
    for (Player& p : gs.players) {
        for (Ability& ab : p.abilities) {
            if (ab.target.size() < 1) continue;
            if (!p.alive && !ab.ghost) continue;
            for (string phase : phases) {
                if (phase == ab.time) {
                    nlohmann::json j;
                    j["type"] = "ShowAbility";
                    j["name"] = ab.name;
                    j["cost"] = ab.cost;
                    j["cooldown"] = ab.cooldown;
                    j["charges"] = ab.charges;
                    j["target"] = ab.target;
                    j["id"] = ab.id;
                    if (ls.conns.find(p.id) != ls.conns.end()) {
                        ls.conns[p.id]->send_text(j.dump());
                    }
                }
            }
        }
    }
}

vector<string> factionalKills {
    "Mafia",
};

void nextPhase(lobbyState& ls, GameState& gs) {
    if (!gs.started) return;
    nlohmann::json j;
    j["type"] = "ClearAbs";
    for (Player& p : gs.players) if (ls.conns.find(p.id) != ls.conns.end()) {
        ls.conns[p.id]->send_text(j.dump());
    }
    j["type"] = "ChangeState";
    if (gs.phase == "Election") {
        procAbs(gs, {"Election"}, ls);
        vector<int> voteCount(gs.players.size(), 0);
        for (int i = 0; i < gs.players.size(); ++i) {
            Player& p = gs.players[i];
            if (p.vote > -1) {
                ++voteCount[p.vote];
                if (gs.mayor == p.localId) ++voteCount[p.vote];
            }
            p.vote = -1;
        }
        int maxVotes = 0, elected = -1;
        
        for (int i = 0; i < voteCount.size(); ++i) {
            if (voteCount[i] > maxVotes) {
                elected = i;
                maxVotes = voteCount[i];
            }
        }
        if (elected == -1) {
            for (Player& p : gs.players) {
                send(ls, p, "[red]No votes were cast. We're redoing the election. Town, if you're purposely stalling for time remember that any evil can vote anyone and in the case of a tie the lower player number wins it.");
            }
        } else {
            gs.mayor = elected;
            for (Ability a : gs.players[elected].role.parliament) {
                gs.players[elected].abilities.push_back(a);
            }
            updatePlayerList(ls, gs.lobbyCode);
            procAbs(gs, {"OnElect"}, ls);
            gs.phase = "Day";
        }
        gs.timer = 30;
    } else if (gs.phase == "Day") {
        procAbs(gs, {"Day"}, ls);
        gs.phase = "Voting";
        ++gs.dayNum;
        ++gs.condemns;
        gs.timer = 30;
    } else if (gs.phase == "Voting") {
        procAbs(gs, {"Voting"}, ls);
        vector<int> voteCount(gs.players.size(), 0);
        map<string, vector<int>> facVotes;
        map<string, int> soldiers;
        for (int i = 0; i < gs.players.size(); ++i) {
            Player& p = gs.players[i];
            if (p.vote > -1) {
                ++voteCount[p.vote];
                if (gs.mayor == p.localId) ++voteCount[p.vote];
            }
            if (p.factionalVote > -1) {
                facVotes[p.alignment].push_back(p.factionalVote);
                soldiers[p.alignment] = find(gs.players.begin(), gs.players.end(), p) - gs.players.begin();
                cout << p.username << " voted for " << p.factionalVote << " (alignment: " << p.alignment << ")\n";
            }
            p.vote = -1;
        }
        int yeee = -1;
        if (gs.dayNum > 0) {
            for (auto& [faction, votex] : facVotes) {
                int maxVotes = 0, player = -1;
                map<int, int> votes;
                for (int i : votex) {
                    ++votes[i];
                }
                for (auto& [plr, voteCount] : votes) {
                    if (voteCount > maxVotes) {
                        player = plr;
                        maxVotes = voteCount;
                    }
                }
                if (player > -1) soldiers[faction] = player;
                Ability ab = killAb;
                ab.id = yeee;
                gs.players[soldiers[faction]].abilities.push_back(ab);
                --yeee;
            }
        }
        
        int maxVotes = 0, exing = -1;
        
        for (int i = 0; i < voteCount.size(); ++i) {
            if (voteCount[i] > maxVotes) {
                exing = i;
                maxVotes = voteCount[i];
            } else if (voteCount[i] == maxVotes) {
                exing = -1;
            }
        }
        
        if (exing > -1) {
            kill(ls, gs, gs.players[exing], 3);
            if (exing == gs.mayor && gs.players[exing].alignment != "Town" && gs.players[exing].alive && gs.mayor == gs.starting) {
                ++gs.condemns;
            }
        }
        --gs.condemns;
        if (gs.condemns > 0) {
            for (Player& p : gs.players) {
                send(ls, p, "[yellow]You have another condemn available! Get voting!");
            }
        } else {
            gs.phase = "Night";
        }
        gs.timer = 30;
    } else if (gs.phase == "Night") {
        endNight(gs, ls);
        gs.timer = 5 * gs.players.size();
        if (!gs.players[gs.mayor].alive) {
            for (Player& p : gs.players) {
                send(ls, p, "[red]The Mayor died! You must elect a new one, NOW!");
            }
            gs.phase = "Election";
        } else {
            gs.phase = "Day";
        }
        
        gs.condemns = 0;
    } else {

    }
    j["state"] = gs.phase;
    j["dayNum"] = gs.dayNum;
    for (Player& p : gs.players) if (ls.conns.find(p.id) != ls.conns.end()) {
        ls.conns[p.id]->send_text(j.dump());
    }
    if (gs.dayNum > 0 && gs.phase == "Night") {
        showAbs(ls, gs, {"Night", "Night*"});
    } else {
        showAbs(ls, gs, {gs.phase});
    }
    
}

void tick(lobbyState& ls) {
    for (auto& [code, gs] : ls.gamestates) {
        gs.timer -= 0.1;
        if (gs.timer <= 0) {
            nextPhase(ls, gs);
        }
    }
}

void startGame(lobbyState& ls, GameState& gs) {
    vector<Player>& players = gs.players;
    vector<string> rolelist = gs.rolelist.generate(players.size());
    random_device rd;
    mt19937 g(rd());
    
    shuffle(players.begin(), players.end(), g);
    shuffle(rolelist.begin(), rolelist.end(), g);
    
    for (int i = 0; i < players.size(); ++i) {
        int abId = 0;
        players[i].role = roles[rolelist[i]];
        players[i].alignment = players[i].role.alignment;
        players[i].localId = i;
        players[i].alive = true;
        for (Ability ab : players[i].role.abs) {
            ab.id = abId;
            players[i].abilities.push_back(ab);
            ++abId;
        }
        for (Ability ab : globalAbs) {
            ab.id = abId;
            players[i].abilities.push_back(ab);
            ++abId;
        }
        if (players[i].alignment == "Mafia") {
            Ability ab = Ability {
                name: "Soldier Vote",
                time: "Voting",
                target: {"alive"},
                func: 10,
                id: abId,
            };
            players[i].income += 2;
            players[i].abilities.push_back(ab);
            ++abId;
        }
        nlohmann::json j;
        j["type"] = "ShowRole";
        j["role"] = players[i].role.name;
        ls.conns[players[i].id]->send_text(j.dump());
        giveCoins(ls, gs, players[i], 0);
        ls.playerIds[players[i].id] = i;
    }
    for (string faction : factionalKills) {
        vector<Player> list;
        for (Player& p : players) {
            if (p.alignment == faction) {
                list.push_back(p);
            }
        }
        for (Player& p : players) {
            if (p.alignment == faction) {
                for (Player s : list) {
                    showRole(ls, gs, p, s, s.role.name);
                }
            }
        }
    }
    
    resetAbs(gs);
    vector<int> mayorPoss(0, 0);
    for (int i = 0; i < gs.players.size(); ++i) {
        if (contains(buckets["Mayor"], gs.players[i].role.name)) mayorPoss.push_back(i);
    } 
    gs.started = true;
    gs.dayNum = 0;
    gs.phase = "Voting";
    gs.mayor = mayorPoss.empty() ? 0 : mayorPoss[rand(0, mayorPoss.size())];
    for (Ability a : gs.players[gs.mayor].role.parliament) {
        gs.players[gs.mayor].abilities.push_back(a);
    }
    procAbs(gs, {"Initial", "OnElect"}, ls);
    gs.starting = gs.mayor;
    gs.timer = 0;
    updatePlayerList(ls, gs.lobbyCode);
    nextPhase(ls, gs);
}
string db_api_key = "sb_secret_6XFWaWtLrNhggNLsOTreuQ_coxeuOQJ", api_url = "https://oidefkjyhtlcahcuuthk.supabase.co";

string table = "Data";

// class SupabaseClient {
//     private:
//         string project_url;
//         string api_key;
//     public:
//         SupabaseClient() {
//             project_url = api_url.substr(api_url.find("://") + 3);
//             api_key = db_api_key;
//         }
//         int changeStat(int id, string stat, string type, int val) {
//             nlohmann::json payload = {
//                 {"player_id", id},
//                 {"column_name", stat},
//                 {"amount", val}
//             };
//             httplib::Client cli(project_url);
//             cli.set_follow_location(true);
        
//             httplib::Headers headers = {
//                 {"apikey", api_key},
//                 {"Authorization", "Bearer " + api_key},
//                 {"Content-Type", "application/json"}
//             };
//             if (type == "ADD") {
//                 auto res = cli.Post("/rest/v1/rpc/increment_stat", 
//                         headers,
//                         payload.dump(),
//                         "application/json");
//                 if (res) {
//                     return res->status;
//                 }
//             } else {
//                 string path = "/rest/v1/" + table + "?" + stat + "=eq." + to_string(val);
//                 auto res = cli.Get(path.c_str(), headers);
            
//                 if (res) {
//                     return res->status;
//                 }
//             }
//         }
// };


int main() {
    
    fillBuckets();
    cout << "Build version: " << __DATE__ << " " << __TIME__ << endl;
#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        cerr << "WSAStartup failed.\n";
        return 1;
    }
#endif
    unordered_map<string, string> passwords;
    unordered_map<string, string> tempId;

    App<CORS> app;
    vector<string> players;
    lobbyState state;
    
    app.route_dynamic("/<path>").methods("OPTIONS"_method)
    ([](const request& req, string path){
        cout << "Received request for: /" << path << ", method: " << (int)req.method << endl;
        if (req.method == HTTPMethod::Options) {
            response res;
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, X-Requested-With, authToken");
            return res;
        }
        return response(404);
    });
    CROW_ROUTE(app, "/ping").methods("GET"_method) 
    ([] {
        return response("pong");
    });

    CROW_ROUTE(app, "/login")
    ([&passwords] (const request& req) {
        // auto body = json::load(req.body);
        // if (!body || !body["username"] || !body["password"]) return response(400, "Malformed login request. Please contact Cyanberry if you see this because you're not supposed to.");
        // string username = body["username"].s();
        // string password = body["password"].s();
        // if (bcrypt_checkpw(password.c_str(), passwords[username].c_str())) {
        //     return response(200, "Login successful, I believe.");
        // } else {
        //     return response(401, "Login failed, probably because your username or password is wrong.");
        // }
        return response(418, "I'm a Coffee pot.");
    });

    CROW_ROUTE(app, "/auth").methods("POST"_method) ([&state](const request& req){
        cout<<"REQUEST RECEIVED"<<endl;
        cout << "[/auth] &state: " << &state << endl;
        auto body = json::load(req.body);
        if (!body || !body.has("player_id")) {
            return response(400, "Missing player_id");
        }

        string player_id = body["player_id"].s();
        string token = generateToken();

        state.tokenRef[token] = player_id;

        cout << "Generated token: " << token << endl;

        json::wvalue res;
        res["token"] = token;
        return response(token);
    });

    CROW_WEBSOCKET_ROUTE(app, "/listen")
    .onopen([&](websocket::connection& conn) {
        
    })
    .onmessage([&state](websocket::connection& conn, const string& data, bool is_binary) {
        cout<<"yes received"<<endl;
        auto msg = json::load(data);
        if (!msg || !msg.has("type")) return;

        string type = msg["type"].s();

        if (type == "auth") {
            string token = msg["token"].s();
            string id = msg["id"].s();
            if (state.tokenRef[token] == id) {
                string lobbyId = state.lobbies[id];
                int playerId = state.playerIds[id];
                state.conns[id] = &conn;
                cout<<"authorized"<<endl; //doesn't print?
            } else {
                conn.close("Unauthorized");
            }
        } else if (type == "chat") {
            string message = msg["msg"].s();
            string id = msg["id"].s();
            int localId = state.playerIds[id];
            string lobby = state.lobbies[id];
            vector<Player>& players = state.gamestates[lobby].players;
            Player& p = players[localId];
            istringstream iss(message);
            string cmd;
            iss>>cmd;
            if (cmd == "/start") {
                startGame(state, state.gamestates[lobby]);
            } else if (cmd == "/w") {
                string sec;
                iss>>sec;
                if (!isNumber(sec)) {
                    p.chat.push_back({-1, "[red]Incorrect usage of /w! The correct usage is \"/w [number] [message]\""});
                    updateChat(state, p);
                    return; 
                }
                int plr = stoi(sec);
                if (plr <= 0 || plr > players.size()) {
                    p.chat.push_back({-1, "[red]That player doesn't exist!"});
                    updateChat(state, p);
                    return; 
                }
                if (plr - 1 == localId) {
                    p.chat.push_back({-1, "[red]For what reason would you whisper yourself?"});
                    updateChat(state, p);
                    return; 
                }
                string sent;
                getline(iss, sent);
                if (!sent.empty() && sent[0] == ' ') {
                    sent.erase(0, 1);
                }
                for (Player& player : players) {
                    player.chat.push_back({-1, "[blue]" + to_string(localId + 1) + " " + p.username + " is whispering to " + to_string(plr) + " " + players[plr - 1].username + "."});
                    bool flag = false;
                    for (Ability& a : player.abilities) {
                        if (a.func == 26) flag = true;
                    }
                    if (player.localId == localId || player.localId == plr - 1 || flag) player.chat.push_back({localId, "[purple]" + sent});
                    updateChat(state, player);
                }
            } else {
                for (Player& player : players) {
                    player.chat.push_back({localId, message});
                    updateChat(state, player);
                }
            }
            
            
        } else if (type == "ability") {
            int abId = msg["id"].i();
            string pId = msg["p_id"].s();
            auto targ = msg["targ"].lo();
            vector<int> parsed;
            for (json::rvalue e : targ) {
                parsed.push_back(e.i());
            }
            int localId = state.playerIds[pId];
            string lobby = state.lobbies[pId];
            GameState& gs = state.gamestates[lobby];
            Player& p = gs.players[localId];
            cout<<"Client updating "<<abId<<endl;
            for (Ability& ab : p.abilities) {
                if (ab.id == abId) {
                    cout<<"Found ability "<<ab.name<<endl;
                    ab.currTarg = parsed;
                }
            }
        }
    })
    .onclose([&state](websocket::connection& conn,
     const string& data,
     uint16_t code) {
        for (pair<string, GameState> gs : state.gamestates) {
            for (Player& p : gs.second.players) {
                leave(p.id, state);
            }
        }
        
    });

    CROW_ROUTE(app, "/join/<string>").methods("POST"_method) ([&state](const request& req, string mode) {

        auto body = json::load(req.body);
        if (!body) {
            return response(400, "Malformed JSON");
        }
        if (!body.has("id") || !body.has("authToken")) {
            return response(400, "Missing id or authToken");
        }

        string id = body["id"].s();
        string token = body["authToken"].s();
        
        auto it = state.tokenRef.find(token);
        if (it == state.tokenRef.end() || it->second != id) {
            return response(401, "Unauthorized: Missing or invalid token");
        }
        if (mode.size() < 3 || mode.size() > 64) {
            return response(400, "Invalid lobby mode");
        }
        response rest;
        rest.code = 200;
        rest.set_header("Content-Type", "text/plain");
        try {
            rest.body = join(mode, id, state);
        } catch (const exception& e) {
            return response(500, "Internal server error");
        }
        return rest;
    });

    thread tickLoop([&state] {
        while (true) {
            tick(state);
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    });
    tickLoop.detach();
    const char* port_env = getenv("PORT");
    int port = port_env ? stoi(port_env) : 18080;
    app.port(port).run();
    #ifdef _WIN32
    WSACleanup();
    #endif
    return 0;
}