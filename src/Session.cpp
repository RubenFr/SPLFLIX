#include "../include/Session.h"

//Variables in the class:
//std::vector<Watchable*> content;
//std::vector<BaseAction*> actionsLog;
//std::unordered_map<std::string, User*> userMap;
//User* activeUser;

//Constructor
Session::Session(const std::string &configFilePath) {
    activeUser = nullptr;
    action_in = "";
    action = {};

    //read the json file
    std::ifstream i(configFilePath);
    nlohmann::json j;
    i >> j;
    nlohmann::json movies = j["movies"];
    nlohmann::json series = j["tv_series"];

    long id = 1;

    for (int i = 0; i < movies.size(); i++) {
        std::vector<std::string> tags;
        std::string name = movies[i]["name"];
        int length = movies[i]["length"];

        for (int j = 0; j < movies[i]["tags"].size(); j++) {
            tags.push_back(movies[i]["tags"][j]);
        }

        Watchable *movie = new Movie(id, name, length, tags);
        content.push_back(movie);
        id++;
    }

    for (int i = 0; i < series.size(); i++) {
        std::vector<std::string> tags;
        std::string name = series[i]["name"];
        int length = series[i]["episode_length"];

        for (int j = 0; j < series[i]["tags"].size(); j++) {
            tags.push_back(series[i]["tags"][j]);
        }
        for (int S = 0; S < series[i]["seasons"].size(); S++) {
            for (int E = 1; E <= series[i]["seasons"][S]; E++) {
                Watchable *episode = new Episode(id, name, length, S+1, E, tags);
                content.push_back(episode);
                id++;
            }
        }
    }
}

//Destructor
Session::~Session()
{
    for (std::vector<Watchable*>::iterator it = content.begin(); it != content.end(); ++it){
        delete *it;
    }
    for (std::vector<BaseAction*>::iterator it = actionsLog.begin(); it != actionsLog.end(); ++it){
        delete *it;
    }
    delete activeUser;
}

//Copy constructor
Session::Session(const Session &other) : content(other.getContent()), actionsLog(other.getActionsLog()), userMap(other.getUserMap()),
    activeUser(other.getActiveUser()) {}

//Copy assignment operator
Session &Session::operator=(Session &other) {
    return *this;
}

//Move constructor
Session::Session(Session &&other) {}

//Move assignment operator
Session &Session::operator=(Session &&other) {
    getAction();
    return *this;
}

void Session::start() { return; }

//getters
std::vector<Watchable*> Session::getContent() const { return content;}
std::vector<BaseAction*> Session::getActionsLog() const { return actionsLog; }
std::unordered_map<std::string, User*> Session::getUserMap() const { return userMap; }
User* Session::getActiveUser() const { return activeUser; }
std::vector<std::string> Session::getAction() { return action; }


