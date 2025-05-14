#include "Registry.h"
#include <iostream>

Registry& Registry::Instance() {
    static Registry instance;
    return instance;
}

void Registry::Register(const std::string& name, CreatorFunc func) {
    registry_[name] = func;
}

const std::map<std::string, Registry::CreatorFunc>& Registry::GetRegistry() const {
    return registry_;
}
