import QtQuick
import QtQuick.Window
import Contactspackage


Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Contact Book")
    color: "#f0f1f2"

    ListView {
            anchors {fill: parent; margins: 20}

            spacing: 3
            clip: true

            model: ContactModel{}

            header: headerDelegate

            delegate: contactsDelegate

            section.property: "contactName"
            section.delegate: sectionDelegate
            section.criteria: ViewSection.FirstCharacter
    }

    Component {
        id: headerDelegate

        Rectangle {
            width: root.width; height: 30

            Text {
                anchors.fill: parent
                text: "Contacts"
                font.bold: true
            }
        }

    }

    Component {
        id: contactsDelegate

        Rectangle {
            width: ListView.view.width; height: 40

            Text {
                id: contactName
                text: model.contactName
            }
            Text {
                anchors.top: contactName.bottom
                text: model.contactNumber
                font.pointSize: 12
            }
        }
    }

    Component {
            id: sectionDelegate

            Rectangle {
                id: sectionWrapper

                required property string section

                width: ListView.view ? ListView.view.width : 0
                height: 20

                color: "white"

                Text {
                    text: sectionWrapper.section
                    color: "grey"
                    font.pointSize: 12
                }
            }
        }
}





