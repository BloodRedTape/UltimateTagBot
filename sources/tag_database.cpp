#include "tag_database.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include "utils.hpp"

using json = nlohmann::json;

JsonTagDatabase::JsonTagDatabase(const char *filename):
    m_Filename(filename)
{
    std::ifstream t(m_Filename);
    if(!t.is_open()){
        puts("Db is not created");
        return;
    }

    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size); 
    if(buffer.size())
        m_Map = json::parse(buffer);
}

void JsonTagDatabase::CreateKeytag(int64_t chat_id, const std::string &keytag){
    m_Map[chat_id][keytag];

    SaveToFile();
}

bool JsonTagDatabase::HasKeytag(int64_t chat_id, const std::string &keytag){
    TagMap &tag_map = m_Map[chat_id];

    return tag_map.find(keytag) != tag_map.end();
}

void JsonTagDatabase::DestroyKeytag(int64_t chat_id, const std::string &keytag){
    m_Map[chat_id].erase(keytag);

    SaveToFile();
}

bool JsonTagDatabase::HasTag(int64_t chat_id, const std::string &keytag, const std::string &tag){
    if(!HasKeytag(chat_id, keytag))
        return false;
    TagSet &tag_set = m_Map[chat_id][keytag];

    return tag_set.find(tag) != tag_set.end(); 
}

void JsonTagDatabase::AddTagsFor(int64_t chat_id, const std::string &keytag, const std::vector<std::string> &tags){
    TagSet &tag_set = m_Map[chat_id][keytag];

    for(const auto &tag: tags)
        tag_set.insert(tag);

    SaveToFile();
}

void JsonTagDatabase::RemoveTagsFor(int64_t chat_id, const std::string &keytag, const std::vector<std::string> &tags){
    TagSet &tag_set = m_Map[chat_id][keytag];

    for(const auto &tag: tags)
        tag_set.erase(tag);
    
    SaveToFile();
}
const TagSet &JsonTagDatabase::GetTagsFor(int64_t chat_id, const Keytag &keytag){
    return m_Map[chat_id][keytag];
}

std::string JsonTagDatabase::GetTaggingMessage(int64_t chat_id, const std::string &keytag){
    if(!HasKeytag(chat_id, keytag)) 
        return {};
    TagSet &set = m_Map[chat_id][keytag];

    std::string message;

    for(const auto &tag: set)
        message += '@' + tag + ' ';

    return message;
}

std::string JsonTagDatabase::GetKeytagsList(int64_t chat_id){
    std::string res;
    for(const auto &[keytag, _]: m_Map[chat_id])
        res += keytag + ' ';
    return res;
}

std::string JsonTagDatabase::GetTagsList(int64_t chat_id, const std::string &keytag){
    if(!HasKeytag(chat_id, keytag))
        return {};

    std::string res;    
    for(const auto &tag: m_Map[chat_id][keytag])
        res += tag + ' ';

    return res;
}

void JsonTagDatabase::SaveToFile(){
    std::string content = json(m_Map).dump();

    std::ofstream stream(m_Filename);
    if(!stream.is_open())
        return (void)puts("Can't open json database");
    stream.write(content.data(), content.size());
}
