#include "pobjectproperty.h"

QList<QStringList> PObjectPropertys::namesItems;   // наименования параметров



void PObjectPropertys::initNamesItems()
{
    QStringList list;
    // анпары TYPE_PROP_ANPAR
    list
        <<"APTag"
        <<"APUnit"
        <<"DPTag"
        <<"DPUnit"
        <<"Name_IsIntegrated"
        <<"Name_Size"
        <<"Name_BackColor"
        <<"Name_ForeColor"
        <<"Name_Font"
        <<"Name_Pos"
        <<"Name"
        <<"Val_BackColor"
        <<"Val_ForeColor"
        <<"Val_Font"
        <<"Val_Type";

    namesItems<<list;

    //TYPE_PROP_TEXT
    list.clear();
    list
         <<"Help";

   namesItems<<list;

   // TYPE_PROP_TEXT_STATE
   list.clear();
   list
       <<"DPTag"
       <<"DPUnit"
       <<"Text"
       <<"Font"
       <<"BackColor"
       <<"ForeColor"
       <<"IsTransparent"
       <<"Is3D"
       <<"Align_V"
       <<"Align_H";

   namesItems<<list;

   // TYPE_PROP_BUTTON
   list.clear();
   list
       <<"Help"
       <<"IsPic"
       <<"BMP_Enabled"
       <<"BMP_Disabled"
       <<"Text"
       <<"Font"
       <<"DPStateTag"
       <<"DPStateUnit"
       <<"Link_Unit"
       <<"Link_Mnemo"
       <<"Link_Act"
       <<"DPPermitTag"
       <<"DPPermitUnit"
       <<"ToolTip_Enabled"
       <<"ToolTip_Disabled"
       <<"ConfirmText"
       <<"DPCmdTag"
       <<"DPCmdUnit"
   ;
   namesItems<<list;

   //TYPE_PROP_IM
   list.clear();
   list
       <<"CenterX"                     // Смещение условного центра по горизонтали
       <<"CenterY"                     // Смещение условного центра по вертикали
       <<"ControlType"                 // Управление сигналом
       <<"CmdNum"                      //Число команд
       <<"StatesCount" 				   //Число состояний объекта
       <<"StatesParCount" 			   //Число управляющих сигналов
       <<"Visible"					   //Признак того, что объект видим
       <<"Help"  				       //Справка по по пиктограммному объекту
       <<"Name"                        //Имя пиктограммного объекта
       <<"Text"                        //Длинное имя пиктограммного объекта (подпись)
       <<"Text_ForeColor"  			   //Цвет наименования
       <<"Text_Font" 				   //Шрифт
       <<"Text_Pos"                    //Положение наименования
       <<"DPRectTag" 				   //Цвет рамки (в формате RGB)
       <<"DPRectUnit" 				   //
       <<"Rect_Color" 				   // Цвет рамки
       <<"Rect_Width" 				   // Толщина рамки
       <<"Rect_Blink" 				   // Период мигания рамки, мс (0 - без мигания)
       <<"Icon"                        // Иконка объекта
      ;
   namesItems<<list;

   //TYPE_PROP_IM_STATE
   list.clear();
   list
       <<"StateFrameIsCycled"          //
       <<"StateFrameDelay"             //
       <<"Frame1"                      //
       <<"Frame2"                      //
       <<"Frame3"                      //
       <<"Frame4"                      //
      ;
   namesItems<<list;



}



