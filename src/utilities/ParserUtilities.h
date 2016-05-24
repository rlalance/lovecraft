#ifndef _PARSER_UTILITIES_H_
#define _PARSER_UTILITIES_H_

#include <utility/YiDateTime.h>
#include <rapidjson/document.h>

enum JSON_VALUE_POLICY
{
    MANDATORY,
    OPTIONAL
};

namespace ParserUtilities
{
    CYIString ParseStringField(const yi::rapidjson::Value &rDocument, const YI_CHAR *pFieldName, bool *pbOk);
    CYIDateTime ParseDateField(const yi::rapidjson::Value &rDocument, const YI_CHAR *pFieldName, bool *pbOk);
    YI_INT32 ParseIntegerField(const yi::rapidjson::Value &rDocument, const YI_CHAR *pFieldName, bool *pbOk);
    YI_FLOAT ParseFloatField(const yi::rapidjson::Value &rDocument, const YI_CHAR *pFieldName, bool *pbOk);
    std::vector<CYIString> ParseStringArrayField(const yi::rapidjson::Value &rDocument, const YI_CHAR *pFieldName, bool *pbOk);
}

#endif
