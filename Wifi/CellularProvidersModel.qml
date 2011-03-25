import Qt 4.7

XmlListModel {
    property string providerName
    source: "/usr/share/mobile-broadband-provider-info/serviceproviders.xml"
    query:  "/serviceproviders/country/provider[name = '"+providerName+"']"

    XmlRole {
        name: "apn"
        query: "gsm/apn[@value]/string()"
    }

    XmlRole {
        name: "username"
        query: "gsm/apn/username/string()"
    }

    XmlRole {
        name: "username"
        query: "gsm/apn/password/string()"
    }
}
