#include "pobjectproperty.h"

QList<QStringList> PObjectPropertys::namesItems;   // ������������ ����������



void PObjectPropertys::initNamesItems()
{
    QStringList list;
    // ������ TYPE_PROP_ANPAR
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
       <<"CenterX"                     // �������� ��������� ������ �� �����������
       <<"CenterY"                     // �������� ��������� ������ �� ���������
       <<"ControlType"                 // ���������� ��������
       <<"CmdNum"                      //����� ������
       <<"StatesCount" 				   //����� ��������� �������
       <<"StatesParCount" 			   //����� ����������� ��������
       <<"Visible"					   //������� ����, ��� ������ �����
       <<"Help"  				       //������� �� �� �������������� �������
       <<"Name"                        //��� �������������� �������
       <<"Text"                        //������� ��� �������������� ������� (�������)
       <<"Text_ForeColor"  			   //���� ������������
       <<"Text_Font" 				   //�����
       <<"Text_Pos"                    //��������� ������������
       <<"DPRectTag" 				   //���� ����� (� ������� RGB)
       <<"DPRectUnit" 				   //
       <<"Rect_Color" 				   // ���� �����
       <<"Rect_Width" 				   // ������� �����
       <<"Rect_Blink" 				   // ������ ������� �����, �� (0 - ��� �������)
       <<"Icon"                        // ������ �������
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



