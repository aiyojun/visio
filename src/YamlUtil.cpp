//
// Created by aiyo on 2019/11/22.
//

#include "YamlUtil.h"
#include <exception>
#include <glog/logging.h>

YAML::Node* YamlUtil::p_cfg = nullptr;

const YAML::Node& YamlUtil::create(std::string path) {
    try {
        YamlUtil::p_cfg = new YAML::Node(YAML::LoadFile(path));
    } catch (...) {
        LOG(ERROR) << "Cannot find visio.yml.";
    }
    return *p_cfg;
}

const YAML::Node& YamlUtil::getYamlFile() {
    return *p_cfg;
}

const YAML::Node &YamlUtil::setDefaultSettings() {
    try {
        if (p_cfg == nullptr) {
            LOG(ERROR) << "please check visio.yml config file.";
            exit(1);
        }
        YAML::Node& refCfg = *p_cfg;
    } catch (...) {

    }
    return *p_cfg;
}
