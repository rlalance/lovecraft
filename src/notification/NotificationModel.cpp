#include "notification/NotificationModel.h"

#include <utility/YiParsingError.h>
#include <utility/YiTranslation.h>

static const CYIString TAG = "NotificationModel";

NOTIFICATION NotificationModel::NotificationFromErrorString(const CYIString &rNotificationString)
{
    static std::map<CYIString, NOTIFICATION> map;

    if (map.empty())
    {
        map["QUEST_AVAILABLE"]                      = QUEST_AVAILABLE;
        map["QUEST_ACCEPTED"]                       = QUEST_ACCEPTED;
        map["QUEST_COMPLETED"]                      = QUEST_COMPLETED;
        map["QUEST_OBJECTIVE_COMPLETED"]            = QUEST_OBJECTIVE_COMPLETED;
        map["INVALID_DEEP_LINK_ERROR"]              = INVALID_DEEP_LINK_ERROR;
        map["APP_CONFIGURATION_ERROR"]              = APP_CONFIGURATION_ERROR;
        map["VSTB_SERVER_ERROR"]                    = VSTB_SERVER_ERROR;
        map["VIDEO_PLAYBACK_ERROR"]                 = VIDEO_PLAYBACK_ERROR;
        map["VIDEO_PLAYBACK_CHANNEL_AUTH_ERROR"]    = VIDEO_PLAYBACK_CHANNEL_AUTH_ERROR;
        map["VIDEO_CONTENT_AUTH_LEVEL_ERROR"]       = VIDEO_CONTENT_AUTH_LEVEL_ERROR;
        map["NETWORK_CONNECTION_ERROR"]             = NETWORK_CONNECTION_ERROR;
        map["LOGOUT_CONFIRMATION"]                  = LOGOUT_CONFIRMATION;
        map["LOGOUT_SUCCESS"]                       = LOGOUT_SUCCESS;
        map["LOGOUT_FAIL"]                          = LOGOUT_FAIL;
    }

    const std::map<CYIString, NOTIFICATION>::const_iterator iter = map.find(rNotificationString);
    YI_ASSERT(iter != map.end(), TAG, "Error string does not match a known error");

    return iter->second;
}

NotificationModel::FIELD NotificationModel::FieldFromFieldString(const CYIString &rFieldString)
{
    static std::map<CYIString, FIELD> map;

    if (map.empty())
    {
        map["type"]         = TYPE;
        map["title"]        = TITLE;
        map["message"]      = MESSAGE;
        map["action"]       = ACTION;
        map["dismiss"]      = DISMISS;
        map["autodismiss"]  = AUTODISMISS;
    }

    const std::map<CYIString, FIELD>::const_iterator iter = map.find(rFieldString);
    YI_ASSERT(iter != map.end(), TAG, "Field string does not match a known field");

    return iter->second;
}

CYIAny NotificationModel::DataForFieldFromValue(FIELD eField, const yi::rapidjson::Value &rValue)
{
    switch (eField)
    {
    case TYPE:
    case TITLE:
    case MESSAGE:
    case ACTION:
    case DISMISS:
        return CYIString(rValue.GetString());
    case AUTODISMISS:
        return rValue.GetBool();
    }

    return CYIAny();
}

NotificationModel::NotificationModel(const CYIString &rJSONPath)
    : CYIAbstractDataModel(0, 0)
    , m_JSONPath(rJSONPath)
    , m_bHasFetched(false)
{
}

NotificationModel::~NotificationModel()
{
}

CYIAny NotificationModel::GetData(NOTIFICATION eNotification, FIELD eField) const
{
    const CYIAny data = GetItemData(GetIndex(eNotification, eField));
    
    switch (eField)
    {
        case TYPE:
        case AUTODISMISS:
            break;
        case TITLE:
        case MESSAGE:
        case ACTION:
        case DISMISS:
            return YiTranslate(data.Get<CYIString>());
    }

    return data;
}

void NotificationModel::SetData(NOTIFICATION eNotification, FIELD eField, const CYIAny &rData)
{
    const YI_INT32 nRowCount = GetRowCount();
    if (nRowCount <= eNotification)
    {
        InsertRows(nRowCount, static_cast<YI_INT32>(eNotification) - nRowCount + 1);
    }

    const YI_INT32 nColumnCount = GetColumnCount();
    if (nColumnCount <= eField)
    {
        InsertColumns(nColumnCount, static_cast<YI_INT32>(eField) - nColumnCount + 1);
    }

    SetItemData(GetIndex(eNotification, eField), rData);
}

void NotificationModel::Fetch()
{
    if (!m_bHasFetched)
    {
        std::FILE *pFile = std::fopen(m_JSONPath.GetData(), "rb");
        YI_ASSERT(pFile, TAG, "Could not open %s", m_JSONPath.GetData());
    
        std::string string;
        std::fseek(pFile, 0, SEEK_END);
        string.resize(static_cast<size_t>(std::ftell(pFile)));
        std::fseek(pFile, 0, SEEK_SET);
        std::fread(&string[0], 1, string.size(), pFile);
        std::fclose(pFile);
    
        ParseErrors(string);
    
        m_bHasFetched = true;
    }
}

void NotificationModel::ParseErrors(const CYIString &rJSONString)
{
    CYIParsingError error;
    yi::rapidjson::Document *pDocument = CYIRapidJSONUtility::CreateDocumentFromString(rJSONString, error);
    YI_ASSERT(!error.HasError(), TAG, "Could not parse: %s", error.GetParsingErrorMessage().GetData());

    yi::rapidjson::Value::ConstMemberIterator iter = pDocument->MemberBegin();
    yi::rapidjson::Value::ConstMemberIterator end = pDocument->MemberEnd();

    for (; iter != end; ++iter)
    {
        ParseError(iter->name.GetString(), iter->value);
    }

    delete pDocument;
}

void NotificationModel::ParseError(const CYIString &rNotification, const yi::rapidjson::Value &rValue)
{
    const NOTIFICATION eNotification = NotificationFromErrorString(rNotification);

    yi::rapidjson::Value::ConstMemberIterator iter = rValue.MemberBegin();
    yi::rapidjson::Value::ConstMemberIterator end = rValue.MemberEnd();

    const CYIString type = rValue["type"].GetString();
    
    bool bCorrectTypeName = ((type == "Dialog") || (type == "Success") || (type == "Error"));
    YI_ASSERT(bCorrectTypeName, TAG, "Incorrect type name: %s", type.GetData());
    
    bool bIsDialogNotificationType = (type == "Dialog");

    for (; iter != end; ++iter)
    {
        const FIELD eField = FieldFromFieldString(iter->name.GetString());
        const CYIAny data = DataForFieldFromValue(eField, iter->value);
        
        bool bInvalidField = !bIsDialogNotificationType && (eField == ACTION || eField == DISMISS);
        YI_ASSERT(!bInvalidField, TAG, "Invalid field");
        
        SetData(eNotification, eField, data);
    }
    
    YI_ASSERT(!GetData(eNotification, NotificationModel::TYPE).Empty(), TAG, "Type field is not found");
    YI_ASSERT(!GetData(eNotification, NotificationModel::TITLE).Empty(), TAG, "Title field is not found");
    YI_ASSERT(!GetData(eNotification, NotificationModel::MESSAGE).Empty(), TAG, "Message field is not found");
    
    if (bIsDialogNotificationType)
    {
        YI_ASSERT(!GetData(eNotification, NotificationModel::ACTION).Empty(), TAG, "Action field is not found");
        YI_ASSERT(!GetData(eNotification, NotificationModel::DISMISS).Empty(), TAG, "Dismiss field is not found");
    }
}
