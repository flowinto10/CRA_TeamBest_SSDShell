#include "Registry.h"
#include <iostream>

Registry& Registry::Instance() {
    static Registry instance;
    std::cout << "(dll)***Registry Instance" << std::endl;
    return instance;
}

void Registry::Register(const std::string& name, CreatorFunc func) {
    std::cout << "(dll)***Registering: " << name << std::endl;
    registry_[name] = func;
}

const std::map<std::string, Registry::CreatorFunc>& Registry::GetRegistry() const {
    std::cout << "(dll)***Getting Registry: " << registry_.size() << std::endl;
    return registry_;
}
