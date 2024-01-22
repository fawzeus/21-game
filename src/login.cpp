#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <string>

class User {
public:
    std::string username;
    std::string password;

    User(const std::string& username, const std::string& password)
        : username(username), password(password) {}
};

int executeQuery(sqlite3* db, const char* sql, std::vector<User>& users) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(
        db,
        sql,
        [](void* data, int argc, char** argv, char** /*azColName*/) -> int {
            std::vector<User>& users = *static_cast<std::vector<User>*>(data);
            users.emplace_back(argv[0], argv[1]);
            return 0;
        },
        &users,
        &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Error executing query: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    return rc;
}

std::vector<User> selectAllUsers(sqlite3* db) {
    std::vector<User> users;
    const char* sql_select_all_users = "SELECT username, password FROM users;";
    int rc = executeQuery(db, sql_select_all_users, users);
    if (rc != SQLITE_OK) {
        std::cerr << "Error selecting all users: " << sqlite3_errmsg(db) << std::endl;
    }
    return users;
}

int addUser(sqlite3* db, const User& user) {
    std::string sql = "INSERT INTO users (username, password) VALUES ('" + user.username + "', '" + user.password + "');";
    return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
}

User getUserByUsername(sqlite3* db, const std::string& username) {
    std::string sql = "SELECT username, password FROM users WHERE username = '" + username + "';";
    std::vector<User> users;
    int rc = executeQuery(db, sql.c_str(), users);
    if (rc != SQLITE_OK || users.empty()) {
        return User("", ""); // Return a default User object indicating not found
    }
    return users[0]; // Return the first (and only) user found
}

int deleteUserByUsername(sqlite3* db, const std::string& username) {
    std::string sql = "DELETE FROM users WHERE username = '" + username + "';";
    return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
}

int updateUserPasswordByUsername(sqlite3* db, const std::string& username, const std::string& newPassword) {
    std::string sql = "UPDATE users SET password = '" + newPassword + "' WHERE username = '" + username + "';";
    return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);
    if (rc) {
        std::cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    const char* sql_create_table = "CREATE TABLE IF NOT EXISTS users ("
                                   "username TEXT PRIMARY KEY,"
                                   "password TEXT NOT NULL);";
    rc = sqlite3_exec(db, sql_create_table, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    User newUser("jane_doe", "password456");
    if (addUser(db, newUser) != SQLITE_OK) {
        std::cerr << "Error adding user." << std::endl;
    }

    User retrievedUser = getUserByUsername(db, "jane_doe");
    if (retrievedUser.username.empty()) {
        std::cerr << "User not found." << std::endl;
    } else {
        std::cout << "Retrieved user: Username: " << retrievedUser.username << ", Password: " << retrievedUser.password << std::endl;
    }

    if (deleteUserByUsername(db, "jane_doe") != SQLITE_OK) {
        std::cerr << "Error deleting user." << std::endl;
    }

    if (updateUserPasswordByUsername(db, "john_doe", "newpassword789") != SQLITE_OK) {
        std::cerr << "Error updating user password." << std::endl;
    }

    std::vector<User> allUsers = selectAllUsers(db);
    for (const auto& user : allUsers) {
        std::cout << "Username: " << user.username << ", Password: " << user.password << std::endl;
    }

    sqlite3_close(db);
    return 0;
}
