import QtQuick
import QtQuick.Window
import Contactspackage
import QtQml.Models

Window {
    //id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Contact Book")
    color: "#f0f1f2"

    Item {
       id: root
       anchors.fill: parent

        ContactModel{
            id:contactmodel
        }

        DelegateModel {
            id: contactsDelegateModel
            model: contactmodel
            delegate: Delegate {}
        }

        ListView {
                id: contactListView
                anchors {fill: root}
                spacing: 5
                clip: true
                model: contactsDelegateModel.parts.list
                interactive: true
                header: Rectangle {
                    width: root.width; height: 30
                    Text {
                        anchors.fill: parent
                        text: "Contacts"
                        font.bold: true
                    }
                }

                section.property: "contactName"
                section.delegate: Rectangle {
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
                section.criteria: ViewSection.FirstCharacter
        }

        ListView {
            id: detailView
            anchors {fill: root}
            spacing: 2
            clip: true
            model: contactsDelegateModel.parts.detail
            interactive: false
            onCurrentIndexChanged: {
                    detailView.positionViewAtIndex(currentIndex, ListView.Contain)
            }
        }

        state: "inList"
        states: [
            State {
                name: 'inList'
                PropertyChanges { target: contactListView; interactive: true; visible: true }
                PropertyChanges { target: detailView; visible: false }
            },
            State {
                name: 'inDetail'; extend: 'inList'
                PropertyChanges { target: contactListView; interactive: false; visible: false }
                PropertyChanges { target: detailView;  visible: true }
            }
        ]
    }
}
