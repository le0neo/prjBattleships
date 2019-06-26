object frmChat: TfrmChat
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Schiffeversenken - Chat'
  ClientHeight = 484
  ClientWidth = 787
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 19
  object txbChat: TEdit
    Left = 16
    Top = 16
    Width = 673
    Height = 27
    TabOrder = 0
    OnChange = txbChatChange
  end
  object btnSenden: TButton
    Left = 695
    Top = 17
    Width = 75
    Height = 25
    Caption = 'Senden'
    Enabled = False
    TabOrder = 1
    OnClick = btnSendenClick
  end
  object btnSchliessen: TButton
    Left = 695
    Top = 441
    Width = 75
    Height = 25
    Caption = 'Schlie'#223'en'
    TabOrder = 2
    OnClick = btnSchliessenClick
  end
  object mChat: TMemo
    Left = 16
    Top = 49
    Width = 673
    Height = 417
    ReadOnly = True
    TabOrder = 3
  end
end
