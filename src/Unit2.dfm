object TodayMsgForm: TTodayMsgForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #26412#26085#12398#12362#30693#12425#12379
  ClientHeight = 344
  ClientWidth = 256
  Color = clBtnFace
  Constraints.MinHeight = 200
  Constraints.MinWidth = 160
  ParentFont = True
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object HaderPaintBox: TPaintBox
    Left = 0
    Top = 0
    Width = 256
    Height = 41
    Align = alTop
    OnPaint = HaderPaintBoxPaint
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 296
    Width = 256
    Height = 48
    Align = alBottom
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 0
    TabStop = True
    object LeftBevel: TBevel
      Left = 0
      Top = 0
      Width = 97
      Height = 48
      Align = alLeft
      Shape = bsSpacer
    end
    object RightBevel: TBevel
      Left = 157
      Top = 0
      Width = 99
      Height = 48
      Align = alClient
      Shape = bsSpacer
    end
    object OkBtnPanel: TPanel
      Left = 97
      Top = 0
      Width = 60
      Height = 48
      Align = alLeft
      AutoSize = True
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 0
      DesignSize = (
        60
        48)
      object OkSpeedButton: TSpeedButton
        Left = 0
        Top = 0
        Width = 60
        Height = 34
        Action = CloseAction
        Anchors = [akBottom]
        Flat = True
      end
    end
  end
  object InfoPanel: TPanel
    Left = 0
    Top = 41
    Width = 256
    Height = 173
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 6
    ParentColor = True
    TabOrder = 1
    object Bevel1: TBevel
      Left = 6
      Top = 47
      Width = 244
      Height = 4
      Align = alTop
      Shape = bsSpacer
    end
    object Bevel2: TBevel
      Left = 6
      Top = 92
      Width = 244
      Height = 4
      Align = alTop
      Shape = bsSpacer
    end
    object Bevel3: TBevel
      Left = 6
      Top = 137
      Width = 244
      Height = 4
      Align = alTop
      Shape = bsSpacer
    end
    object MemPanel4: TPanel
      Left = 6
      Top = 141
      Width = 244
      Height = 26
      Align = alClient
      BevelOuter = bvNone
      BorderWidth = 4
      TabOrder = 0
      object Inf4Memo: TMemo
        Left = 4
        Top = 4
        Width = 236
        Height = 18
        TabStop = False
        Align = alClient
        BorderStyle = bsNone
        ReadOnly = True
        TabOrder = 0
      end
    end
    object MemPanel3: TPanel
      Left = 6
      Top = 96
      Width = 244
      Height = 41
      Align = alTop
      BevelOuter = bvNone
      BorderWidth = 4
      TabOrder = 1
      object Inf3Memo: TMemo
        Left = 4
        Top = 4
        Width = 236
        Height = 33
        TabStop = False
        Align = alClient
        BorderStyle = bsNone
        ReadOnly = True
        TabOrder = 0
      end
    end
    object MemPanel2: TPanel
      Left = 6
      Top = 51
      Width = 244
      Height = 41
      Align = alTop
      BevelOuter = bvNone
      BorderWidth = 4
      TabOrder = 2
      object Inf2Memo: TMemo
        Left = 4
        Top = 4
        Width = 236
        Height = 33
        TabStop = False
        Align = alClient
        BorderStyle = bsNone
        ReadOnly = True
        TabOrder = 0
      end
    end
    object MemPanel1: TPanel
      Left = 6
      Top = 6
      Width = 244
      Height = 41
      Align = alTop
      BevelOuter = bvNone
      BorderWidth = 4
      TabOrder = 3
      object Inf1Memo: TMemo
        Left = 4
        Top = 4
        Width = 236
        Height = 33
        TabStop = False
        Align = alClient
        BorderStyle = bsNone
        ReadOnly = True
        TabOrder = 0
      end
    end
  end
  object MoonPanel: TPanel
    Left = 0
    Top = 214
    Width = 256
    Height = 82
    Align = alBottom
    BevelOuter = bvNone
    BorderWidth = 8
    DoubleBuffered = True
    ParentColor = True
    ParentDoubleBuffered = False
    TabOrder = 2
    object MoonPaintBox: TPaintBox
      Left = 8
      Top = 8
      Width = 73
      Height = 66
      Align = alLeft
      Color = clBlack
      ParentColor = False
      OnPaint = MoonPaintBoxPaint
    end
    object MoonMemo: TMemo
      Left = 81
      Top = 8
      Width = 167
      Height = 66
      TabStop = False
      Align = alClient
      BorderStyle = bsNone
      Color = clBlack
      Lines.Strings = (
        '')
      ReadOnly = True
      TabOrder = 0
    end
  end
  object ActionList1: TActionList
    Left = 8
    Top = 4
    object CloseAction: TAction
      Caption = 'OK'
      ShortCut = 13
      OnExecute = CloseExecute
    end
    object EscAction: TAction
      Caption = 'EscAction'
      ShortCut = 27
      OnExecute = CloseExecute
    end
  end
end
