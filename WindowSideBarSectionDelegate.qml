import Qt 4.7
import MeeGo.Components 0.1

Item {
    id: sectionDelegate
    anchors.left:  parent.left
    anchors.right:  parent.right
    //anchors.top:  parent.top
    height: visible ? 50 : 0 //TODO: may be different

    visible: true

    ThemeImage {
        id: sectionImage

        anchors {
            left: parent.left
            right:parent.right
        }
        height: sectionDelegate.height

        source: "image://themedimage/widgets/common/header/header-inverted-small-top"

        LayoutTextItem {
            id: sectionText

            anchors.fill:  parent
            anchors.margins:  10

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment:Text.AlignLeft
            elide: Text.ElideRight

            text: section
            font.pixelSize: theme.fontPixelSizeLarge

            /*Component.onCompleted: {
                if (model.section != delegateParent.sectionValue) {
                    delegateParent.sectionValue = model.section
                    sectionImage.source = delegateParent.firstSection ? "image://themedimage/widgets/common/header/header-inverted-small-top"
                                                                      : "image://themedimage/widgets/common/header/header-inverted-small"
                    delegateParent.firstSection = false
                    sectionDelegate.visible = true
                }
                else {
                    sectionDelegate.visible = false
                }
            }*/
        }
    }
} //end sectionDelegate
