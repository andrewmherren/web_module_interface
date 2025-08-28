#include "auth_types.h"
#include <algorithm>

namespace AuthUtils {

bool hasAuthType(const AuthRequirements& requirements, AuthType type) {
    return std::find(requirements.begin(), requirements.end(), type) != requirements.end();
}

bool requiresAuth(const AuthRequirements& requirements) {
    // Empty requirements or only NONE means no auth required
    if (requirements.empty()) {
        return false;
    }
    
    // If only NONE is present, no auth required
    if (requirements.size() == 1 && requirements[0] == AuthType::NONE) {
        return false;
    }
    
    // Any other combination requires authentication
    return true;
}

String authTypeToString(AuthType type) {
    switch (type) {
        case AuthType::NONE:
            return "NONE";
        case AuthType::SESSION:
            return "SESSION";
        case AuthType::TOKEN:
            return "TOKEN";
        case AuthType::LOCAL_ONLY:
            return "LOCAL_ONLY";
        default:
            return "UNKNOWN";
    }
}

AuthRequirements none() {
    return {AuthType::NONE};
}

AuthRequirements sessionOnly() {
    return {AuthType::SESSION};
}

AuthRequirements tokenOnly() {
    return {AuthType::TOKEN};
}

AuthRequirements sessionOrToken() {
    return {AuthType::SESSION, AuthType::TOKEN};
}

} // namespace AuthUtils