#include "tag_database.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

using json = nlohmann::json;

JsonTagDatabase::JsonTagDatabase(const char *filename):
    m_Filename(filename)
{
    std::ifstream t(m_Filename);
    if(!t.is_open())
        return;

    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size); 
    m_Map = json::parse(buffer);
}

void JsonTagDatabase::CreateKeytag(int32_t chat_id, const std::string &keytag){
    m_Map[chat_id][keytag];

    SaveToFile();
}

bool JsonTagDatabase::HasKeytag(int32_t chat_id, const std::string &keytag){
    TagMap &tag_map = m_Map[chat_id];

    return tag_map.find(keytag) != tag_map.end();
}

void JsonTagDatabase::DestroyKeytag(int32_t chat_id, const std::string &keytag){
    m_Map[chat_id].erase(keytag);

    SaveToFile();
}

bool JsonTagDatabase::HasTag(int32_t chat_id, const std::string &keytag, const std::string &tag){
    if(!HasKeytag(chat_id, keytag))
        return false;
    TagSet &tag_set = m_Map[chat_id][keytag];

    return tag_set.find(tag) != tag_set.end(); 
}

void JsonTagDatabase::AddTagsFor(int32_t chat_id, const std::string &keytag, const std::vector<std::string> &tags){
    TagSet &tag_set = m_Map[chat_id][keytag];

    for(const auto &tag: tags)
        tag_set.insert(tag);

    SaveToFile();
}

void JsonTagDatabase::RemoveTagsFor(int32_t chat_id, const std::string &keytag, const std::vector<std::string> &tags){
    TagSet &tag_set = m_Map[chat_id][keytag];

    for(const auto &tag: tags)
        tag_set.erase(tag);
    
    SaveToFile();
}

std::string JsonTagDatabase::GetTaggingMessage(int32_t chat_id, const std::string &keytag){
    if(!HasKeytag(chat_id, keytag)) 
        return {};
    
    TagSet &set = m_Map[chat_id][keytag];

    std::string message;

    for(const auto &tag: set)
        message += tag + ' ';

    return message;
}

std::string JsonTagDatabase::GetKeytagsList(int32_t chat_id){
    std::string res;
    for(const auto &[keytag, _]: m_Map[chat_id])
        res += keytag + ' ';
    return res;
}

std::string JsonTagDatabase::GetTagsList(int32_t chat_id, const std::string &keytag){
    if(!HasKeytag(chat_id, keytag))
        return {};

    std::string res;    
    for(const auto &tag: m_Map[chat_id][keytag])
        res += tag.substr(1, tag.size()) + ' ';

    return res;
}

void JsonTagDatabase::SaveToFile(){
    std::string content = json(m_Map).dump();

    std::ofstream stream(m_Filename);
    if(!stream.is_open())
        return (void)puts("Can't open json database");
    stream.write(content.data(), content.size());
}
