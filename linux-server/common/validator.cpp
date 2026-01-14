#include "validator.h"
#include <QRegularExpression>

bool Validator::isValidEmail(const std::string& email) {
    if (email.size() > 254) return false;
    static const QRegularExpression re(
        R"(^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$)"
    );
    return re.match(QString::fromStdString(email)).hasMatch();
}