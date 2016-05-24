#include "utilities/ParserUtilities.h"

#include <utility/YiParsingError.h>
#include <utility/YiRapidJSONUtility.h>

static const CYIString TAG = "ParserUtilities";

namespace ParserUtilities
{
    CYIString ParseStringField(const yi::rapidjson::Value &rDocument, const YI_CHAR *pFieldName, bool *pbOk)
    {
        CYIString value;
        
        CYIParsingError error;
        CYIRapidJSONUtility::GetStringField(&rDocument, pFieldName, value, error);
        if (pbOk)
        {
            *pbOk = !error.HasError();
        }
        
        return value;
    }

    CYIDateTime ParseDateField(const yi::rapidjson::Value &rDocument, const YI_CHAR *pFieldName, bool *pbOk)
    {
        CYIDateTime value;
        
        CYIParsingError error;
        CYIRapidJSONUtility::GetDateField(&rDocument, pFieldName, value, error);
        if (pbOk)
        {
            *pbOk = !error.HasError();
        }
        
        return value;
    }

    YI_INT32 ParseIntegerField(const yi::rapidjson::Value &rDocument, const YI_CHAR *pFieldName, bool *pbOk)
    {
        YI_INT32 nValue = -1;
        
        CYIParsingError error;
        CYIRapidJSONUtility::GetIntegerField(&rDocument, pFieldName, &nValue, error);
        if (pbOk)
        {
            *pbOk = !error.HasError();
        }
        
        return nValue;
    }

    YI_FLOAT ParseFloatField(const yi::rapidjson::Value &rDocument, const YI_CHAR *pFieldName, bool *pbOk)
    {
        YI_FLOAT nValue = -1;
        
        CYIParsingError error;
        CYIRapidJSONUtility::GetFloatField(&rDocument, pFieldName, &nValue, error);
        if (pbOk)
        {
            *pbOk = !error.HasError();
        }
        
        return nValue;
    }

    std::vector<CYIString> ParseStringArrayField(const yi::rapidjson::Value &rDocument, const YI_CHAR *pFieldName, bool *pbOk)
    {
        std::vector<CYIString> array;
        
        if (rDocument.HasMember(pFieldName))
        {
            const yi::rapidjson::Value &field = rDocument[pFieldName];
            
            for (yi::rapidjson::Value::ConstValueIterator iter = field.Begin(); iter != field.End(); ++iter)
            {
                if (iter->IsString())
                {
                    array.push_back(iter->GetString());
                }
                else
                {
                    if (pbOk)
                    {
                        *pbOk = false;
                    }
                    break;
                }
            }
        }
        else if (pbOk)
        {
            *pbOk = false;
        }
        
        return array;
    }
}

