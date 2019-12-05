//
// Created by aiyo on 2019/11/22.
//

#ifndef VISIO_YAMLUTIL_H
#define VISIO_YAMLUTIL_H

#include <yaml-cpp/yaml.h>
#include <string>

class YamlUtil {
public:
    static const YAML::Node& create(std::string path);
    static const YAML::Node& setDefaultSettings();
    static const YAML::Node& getYamlFile();
private:
    static YAML::Node* p_cfg;
};


#endif //VISIO_YAMLUTIL_H
