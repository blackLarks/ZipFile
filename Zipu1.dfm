object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Random Flag Display'
  ClientHeight = 596
  ClientWidth = 800
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  TextHeight = 15
  object ImageFlag: TImage
    Left = 50
    Top = 66
    Width = 700
    Height = 400
    Center = True
    Proportional = True
    Stretch = True
  end
  object LabelFlagName: TLabel
    Left = 50
    Top = 486
    Width = 94
    Height = 28
    Alignment = taCenter
    Caption = 'Flag Name'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -20
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object LabelStatus: TLabel
    Left = 50
    Top = 16
    Width = 115
    Height = 28
    Caption = 'Status: Ready'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -20
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object ButtonRandomFlag: TButton
    Left = 332
    Top = 512
    Width = 120
    Height = 35
    Caption = 'Random Flag'
    TabOrder = 0
    OnClick = ButtonRandomClick
  end
end
