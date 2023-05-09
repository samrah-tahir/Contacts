import QtQuick
import QtQuick.Window
import Contactspackage

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Contact Book")

    Contacts{
        id: contactsid
    }


    Rectangle {
            anchors.fill: parent
            color: "pink"
            ListView {
                    anchors.fill: parent
                    anchors.margins: 20

                    focus: true

                    model: contactsid.contactList
                    delegate: Rectangle {
                        width: parent.width
                        height: 40
                        color: "white"

                        Text {
                            anchors.fill: parent
                            text: modelData
                        }
                    }

                    clip: true
                }

        }

}


