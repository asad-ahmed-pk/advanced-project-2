//
// ConfigParser.hpp
// Parses the config XML file and provides an interface to required attributes
//

#ifndef FUMAROLE_LOCALIZATION_CONFIGPARSER_HPP
#define FUMAROLE_LOCALIZATION_CONFIGPARSER_HPP

#include <string>
#include <memory>
#include <boost/property_tree/ptree.hpp>

namespace Config
{
    class ConfigParser
    {
    public:
        /// Get a reference to the instance
        /// \return the reference to the instance
        static ConfigParser& GetInstance();

        /// Get the value for the config path
        /// \param path path separated by '.'
        template <class T>
        T GetValue(const std::string& path) const {
            return m_PropertyTree->get<T>(path);
        }

        ConfigParser(ConfigParser const&) = delete;
        void operator=(ConfigParser const&) = delete;

        ~ConfigParser(){}

    private:
        ConfigParser() {}

    private:
        std::unique_ptr<boost::property_tree::ptree> m_PropertyTree = nullptr;
    };
}

#endif //FUMAROLE_LOCALIZATION_CONFIGPARSER_HPP
