#ifndef _NOTIFICATION_MODEL_H_
#define _NOTIFICATION_MODEL_H_

#include "notification/Notifications.h"

#include <datamodel/YiAbstractDataModel.h>
#include <utility/YiRapidJSONUtility.h>

class NotificationModel : public CYIAbstractDataModel
{

public:
    enum FIELD
    {
        TYPE,           // CYIString
        TITLE,          // CYIString
        MESSAGE,        // CYIString
        ACTION,         // CYIString
        DISMISS,        // CYIString
        AUTODISMISS     // bool
    };

    NotificationModel(const CYIString &rJSONPath);
    virtual ~NotificationModel();

    void Fetch();

    CYIAny GetData(NOTIFICATION eNotification, FIELD eField) const;

private:
    void SetData(NOTIFICATION eNotification, FIELD eField, const CYIAny &rData);

    void ParseErrors(const CYIString &rJSONString);
    void ParseError(const CYIString &rError, const yi::rapidjson::Value &rValue);

    static NOTIFICATION NotificationFromErrorString(const CYIString &rNotificationString);
    static FIELD FieldFromFieldString(const CYIString &rFieldString);
    static CYIAny DataForFieldFromValue(FIELD eField, const yi::rapidjson::Value &rValue);

    const CYIString m_JSONPath;
    bool m_bHasFetched;

    YI_DISALLOW_COPY_AND_ASSIGN(NotificationModel);
};

#endif // _NOTIFICATION_MODEL_H_

