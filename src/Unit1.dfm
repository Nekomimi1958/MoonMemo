object MoonMemoForm: TMoonMemoForm
  Left = 0
  Top = 0
  HelpContext = 1
  Caption = #26376#12513#12514
  ClientHeight = 471
  ClientWidth = 663
  Color = clBtnFace
  Constraints.MinHeight = 200
  Constraints.MinWidth = 300
  ParentFont = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 241
    Top = 0
    Width = 4
    Height = 452
    AutoSnap = False
    MinSize = 160
    ResizeStyle = rsUpdate
  end
  object CalPanel: TPanel
    Left = 0
    Top = 0
    Width = 241
    Height = 452
    HelpContext = 4
    Align = alLeft
    BevelOuter = bvNone
    DoubleBuffered = True
    Padding.Left = 8
    Padding.Top = 6
    Padding.Right = 6
    Padding.Bottom = 6
    ParentColor = True
    ParentDoubleBuffered = False
    TabOrder = 0
    OnResize = CalPanelResize
    object HeaderPaintBox: TPaintBox
      Left = 8
      Top = 6
      Width = 227
      Height = 85
      Align = alTop
      Color = clBtnFace
      ParentColor = False
      PopupMenu = JumpPopupMenu
      OnPaint = HeaderPaintBoxPaint
    end
    object CalBottomPanel: TPanel
      Left = 8
      Top = 406
      Width = 227
      Height = 40
      Align = alBottom
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 0
      DesignSize = (
        227
        40)
      object BefMonBtn: TSpeedButton
        Left = 106
        Top = 6
        Width = 28
        Height = 28
        Action = BefMonAction
        Anchors = [akRight, akBottom]
        Flat = True
      end
      object TodayBtn: TSpeedButton
        Left = 134
        Top = 6
        Width = 28
        Height = 28
        Action = TodayAction
        Anchors = [akRight, akBottom]
        Flat = True
      end
      object NxtMonBtn: TSpeedButton
        Left = 162
        Top = 6
        Width = 28
        Height = 28
        Action = NxtMonAction
        Anchors = [akRight, akBottom]
        Flat = True
      end
      object NxtYearBtn: TSpeedButton
        Left = 190
        Top = 6
        Width = 28
        Height = 28
        Action = NxtYearAction
        Anchors = [akRight, akBottom]
        Flat = True
      end
      object BefYearBtn: TSpeedButton
        Left = 78
        Top = 6
        Width = 28
        Height = 28
        Action = BefYearAction
        Anchors = [akRight, akBottom]
        Flat = True
      end
      object HelpButton: TSpeedButton
        Left = 0
        Top = 6
        Width = 28
        Height = 28
        Action = HelpAction
        Anchors = [akLeft, akBottom]
        Flat = True
        Spacing = 0
      end
    end
    object CalBtnPanel: TPanel
      Left = 8
      Top = 91
      Width = 227
      Height = 315
      Align = alClient
      BevelOuter = bvNone
      ParentColor = True
      TabOrder = 1
      object CalPaintBox: TPaintBox
        Left = 0
        Top = 0
        Width = 227
        Height = 315
        Align = alClient
        PopupMenu = CalPopupMenu
        OnMouseDown = CalPaintBoxMouseDown
        OnPaint = CalPaintBoxPaint
      end
    end
  end
  object RightBackPanel: TPanel
    Left = 245
    Top = 0
    Width = 418
    Height = 452
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 1
    Padding.Top = 4
    ParentColor = True
    TabOrder = 1
    object RzPageControl1: TRzPageControl
      Left = 1
      Top = 5
      Width = 416
      Height = 446
      Hint = ''
      ActivePage = WeekSheet
      Align = alClient
      BoldCurrentTab = True
      TabIndex = 0
      TabOrder = 0
      OnChange = RzPageControl1Change
      OnChanging = RzPageControl1Changing
      FixedDimension = 19
      object WeekSheet: TRzTabSheet
        HelpContext = 5
        Caption = #36913#38291#20104#23450
        Padding.Left = 4
        Padding.Top = 4
        Padding.Right = 4
        object WekPanel: TPanel
          Left = 4
          Top = 4
          Width = 404
          Height = 379
          Align = alClient
          BevelOuter = bvNone
          DoubleBuffered = True
          ParentColor = True
          ParentDoubleBuffered = False
          TabOrder = 0
          StyleElements = []
          object WekBtnPaintBox: TPaintBox
            Left = 0
            Top = 0
            Width = 105
            Height = 379
            Align = alLeft
            PopupMenu = CalPopupMenu
            OnMouseDown = WekBtnPaintBoxMouseDown
            OnPaint = WekBtnPaintBoxPaint
          end
          object WekSplitter: TSplitter
            Left = 105
            Top = 0
            Height = 379
            AutoSnap = False
            ResizeStyle = rsUpdate
            OnMoved = WekSplitterMoved
          end
          object WekMemPanel: TPanel
            Left = 108
            Top = 0
            Width = 296
            Height = 379
            Align = alClient
            BevelOuter = bvNone
            ParentColor = True
            TabOrder = 0
            StyleElements = []
            OnResize = WekMemPanelResize
            object RzPanel1: TRzPanel
              Left = 72
              Top = 112
              Width = 185
              Height = 41
              Color = 15987699
              TabOrder = 0
            end
          end
        end
        object WekBottomPanel: TPanel
          Left = 4
          Top = 383
          Width = 404
          Height = 40
          Align = alBottom
          BevelOuter = bvNone
          ParentColor = True
          TabOrder = 1
          StyleElements = []
          object WekNavPanel: TPanel
            Left = 0
            Top = 0
            Width = 104
            Height = 40
            Align = alLeft
            BevelOuter = bvNone
            BorderWidth = 4
            Constraints.MinWidth = 104
            ParentColor = True
            TabOrder = 0
            StyleElements = []
            object BefWekBtn: TSpeedButton
              Left = 4
              Top = 4
              Width = 32
              Height = 28
              Action = BefWekAction
              Flat = True
            end
            object NxtWekBtn: TSpeedButton
              Left = 68
              Top = 4
              Width = 32
              Height = 28
              Action = NxtWekAction
              Flat = True
            end
            object TodayBtn2: TSpeedButton
              Left = 36
              Top = 4
              Width = 32
              Height = 28
              Action = TodayAction
              Flat = True
            end
          end
          object WebTabPanel: TPanel
            Left = 104
            Top = 0
            Width = 300
            Height = 40
            Align = alClient
            BevelOuter = bvNone
            ParentColor = True
            TabOrder = 1
            StyleElements = []
            object MemRzTabControl: TRzTabControl
              Left = 0
              Top = 0
              Width = 300
              Height = 27
              Hint = ''
              Align = alTop
              BoldCurrentTab = True
              ShowCard = False
              ShowCardFrame = False
              TabIndex = 0
              TabOrder = 0
              TabOrientation = toBottom
              Tabs = <
                item
                  Caption = #20104#23450'1'
                end
                item
                  Caption = #20104#23450'2'
                end
                item
                  Caption = #35352#24565#26085
                end
                item
                  Caption = #26278#24773#22577
                end>
              TabStyle = tsBackSlant
              OnChange = MemRzTabControlChange
              OnChanging = MemRzTabControlChanging
              FixedDimension = 19
            end
          end
        end
      end
      object Memo1Sheet: TRzTabSheet
        HelpContext = 6
        Caption = #12513#12514
        Padding.Left = 4
        Padding.Top = 4
        Padding.Right = 4
        Padding.Bottom = 4
        object ToDoMemo1: TMemo
          Left = 4
          Top = 4
          Width = 404
          Height = 415
          Align = alClient
          ScrollBars = ssVertical
          TabOrder = 0
          OnEnter = WekMemoEnter
          OnExit = WekMemoExit
        end
      end
      object Memo2Sheet: TRzTabSheet
        HelpContext = 6
        Padding.Left = 4
        Padding.Top = 4
        Padding.Right = 4
        Padding.Bottom = 4
        object ToDoMemo2: TMemo
          Left = 4
          Top = 4
          Width = 404
          Height = 415
          Align = alClient
          ScrollBars = ssVertical
          TabOrder = 0
          OnEnter = WekMemoEnter
          OnExit = WekMemoExit
        end
      end
      object Memo3Sheet: TRzTabSheet
        HelpContext = 6
        Padding.Left = 4
        Padding.Top = 4
        Padding.Right = 4
        Padding.Bottom = 4
        object ToDoMemo3: TMemo
          Left = 4
          Top = 4
          Width = 404
          Height = 415
          Align = alClient
          ScrollBars = ssVertical
          TabOrder = 0
          OnEnter = WekMemoEnter
          OnExit = WekMemoExit
        end
      end
      object FindSheet: TRzTabSheet
        HelpContext = 7
        Caption = #26908#32034
        Padding.Left = 4
        Padding.Top = 4
        Padding.Right = 4
        Padding.Bottom = 4
        object FindBottomPanel: TPanel
          Left = 4
          Top = 378
          Width = 404
          Height = 41
          Align = alBottom
          BevelOuter = bvNone
          ParentColor = True
          TabOrder = 0
          DesignSize = (
            404
            41)
          object FindWdLabel: TLabel
            Left = 8
            Top = 14
            Width = 36
            Height = 13
            Anchors = [akLeft, akBottom]
            Caption = #26908#32034#35486
          end
          object FindBtn: TSpeedButton
            Left = 317
            Top = 8
            Width = 65
            Height = 27
            Action = StartFindAction
            Anchors = [akRight, akBottom]
            Flat = True
          end
          object FindStrComboBox: TComboBox
            Left = 64
            Top = 10
            Width = 250
            Height = 23
            Anchors = [akLeft, akRight, akBottom]
            Font.Charset = SHIFTJIS_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnKeyDown = FindStrComboBoxKeyDown
          end
        end
        object FindListBox: TListBox
          Left = 4
          Top = 4
          Width = 404
          Height = 374
          Align = alClient
          ItemHeight = 13
          PopupMenu = FindPopupMenu
          TabOrder = 1
          OnDblClick = FindListBoxDblClick
          OnKeyDown = FindListBoxKeyDown
        end
      end
      object OptSheet: TRzTabSheet
        HelpContext = 8
        Caption = #35373#23450
        Padding.Left = 2
        Padding.Top = 4
        Padding.Right = 2
        Padding.Bottom = 2
        object OptPanel: TPanel
          Left = 2
          Top = 4
          Width = 408
          Height = 417
          Align = alClient
          BevelOuter = bvNone
          Caption = 'OptPanel'
          Padding.Left = 4
          Padding.Top = 4
          Padding.Right = 4
          Padding.Bottom = 4
          ParentColor = True
          TabOrder = 0
          object RzPageControl2: TRzPageControl
            Left = 4
            Top = 4
            Width = 400
            Height = 409
            Hint = ''
            ActivePage = Opt1TabSheet
            Align = alClient
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = []
            MultiLine = True
            ParentColor = False
            ParentFont = False
            TabIndex = 0
            TabOrder = 0
            TabWidth = 100
            FixedDimension = 19
            object Opt1TabSheet: TRzTabSheet
              HelpContext = 9
              Caption = #12487#12540#12479
              Padding.Left = 8
              Padding.Top = 8
              Padding.Right = 8
              object Bevel2: TBevel
                Left = 8
                Top = 204
                Width = 375
                Height = 6
                Align = alTop
                Shape = bsSpacer
              end
              object Bevel1: TBevel
                Left = 8
                Top = 93
                Width = 375
                Height = 6
                Align = alTop
                Shape = bsSpacer
              end
              object Bevel3: TBevel
                Left = 8
                Top = 293
                Width = 375
                Height = 6
                Align = alTop
                Shape = bsSpacer
              end
              object MemFilGroupBox: TGroupBox
                Left = 8
                Top = 99
                Width = 375
                Height = 105
                Align = alTop
                Caption = #12513#12514#12486#12461#12473#12488#12501#12449#12452#12523
                TabOrder = 1
                DesignSize = (
                  375
                  105)
                object Label1_1: TLabel
                  Left = 12
                  Top = 24
                  Width = 6
                  Height = 13
                  Caption = '1'
                end
                object Label1_2: TLabel
                  Left = 12
                  Top = 50
                  Width = 6
                  Height = 13
                  Caption = '2'
                end
                object Label1_3: TLabel
                  Left = 12
                  Top = 76
                  Width = 6
                  Height = 13
                  Caption = '3'
                end
                object OptMemFile1Edit: TEdit
                  Left = 126
                  Top = 22
                  Width = 213
                  Height = 21
                  Anchors = [akLeft, akTop, akRight]
                  ReadOnly = True
                  TabOrder = 1
                end
                object RefMemo1Btn: TButton
                  Left = 340
                  Top = 21
                  Width = 36
                  Height = 22
                  Anchors = [akTop, akRight]
                  Caption = '...'
                  TabOrder = 2
                  OnClick = RefMemo1BtnClick
                end
                object OptMemFile2Edit: TEdit
                  Left = 126
                  Top = 48
                  Width = 213
                  Height = 21
                  Anchors = [akLeft, akTop, akRight]
                  ReadOnly = True
                  TabOrder = 4
                end
                object RefMemo2Btn: TButton
                  Tag = 1
                  Left = 340
                  Top = 48
                  Width = 36
                  Height = 22
                  Anchors = [akTop, akRight]
                  Caption = '...'
                  TabOrder = 5
                  OnClick = RefMemo1BtnClick
                end
                object OptMemFile3Edit: TEdit
                  Left = 126
                  Top = 74
                  Width = 213
                  Height = 21
                  Anchors = [akLeft, akTop, akRight]
                  ReadOnly = True
                  TabOrder = 7
                end
                object RefMemo3Btn: TButton
                  Tag = 2
                  Left = 340
                  Top = 74
                  Width = 36
                  Height = 22
                  Anchors = [akTop, akRight]
                  Caption = '...'
                  TabOrder = 8
                  OnClick = RefMemo1BtnClick
                end
                object OptMemTit1Edit: TEdit
                  Left = 32
                  Top = 22
                  Width = 90
                  Height = 21
                  TabOrder = 0
                  OnChange = OptMemTitEditChange
                end
                object OptMemTit2Edit: TEdit
                  Tag = 1
                  Left = 32
                  Top = 48
                  Width = 90
                  Height = 21
                  TabOrder = 3
                  OnChange = OptMemTitEditChange
                end
                object OptMemTit3Edit: TEdit
                  Tag = 2
                  Left = 32
                  Top = 74
                  Width = 90
                  Height = 21
                  TabOrder = 6
                  OnChange = OptMemTitEditChange
                end
              end
              object InfFilBox3: TGroupBox
                Left = 8
                Top = 299
                Width = 375
                Height = 50
                Align = alTop
                Caption = #26278#24773#22577#12487#12540#12479#12501#12449#12452#12523
                TabOrder = 4
                DesignSize = (
                  375
                  50)
                object OptInfFileEdit: TEdit
                  Left = 10
                  Top = 20
                  Width = 319
                  Height = 21
                  Anchors = [akLeft, akTop, akRight]
                  ReadOnly = True
                  TabOrder = 0
                  OnKeyDown = OptInfFileEditKeyDown
                end
                object RefInfoBtn: TButton
                  Left = 330
                  Top = 20
                  Width = 36
                  Height = 22
                  Anchors = [akTop, akRight]
                  Caption = '...'
                  TabOrder = 1
                  OnClick = RefInfoBtnClick
                end
              end
              object DatOptPanel: TPanel
                Left = 8
                Top = 260
                Width = 375
                Height = 33
                Align = alTop
                BevelOuter = bvNone
                TabOrder = 3
                object SaveUtf8CheckBox: TCheckBox
                  Left = 10
                  Top = 8
                  Width = 295
                  Height = 17
                  Caption = #12487#12540#12479#12501#12449#12452#12523#12434'Unicode(UTF-8)'#12391#20445#23384
                  TabOrder = 0
                  OnClick = OptionChanged
                end
              end
              object BackupGroupBox: TGroupBox
                Left = 8
                Top = 210
                Width = 375
                Height = 50
                Align = alTop
                Caption = #12496#12483#12463#12450#12483#12503#20808
                TabOrder = 2
                DesignSize = (
                  375
                  50)
                object OptBackupDirEdit: TEdit
                  Left = 10
                  Top = 20
                  Width = 319
                  Height = 21
                  Anchors = [akLeft, akTop, akRight]
                  ReadOnly = True
                  TabOrder = 0
                  OnKeyDown = OptBackupDirEditKeyDown
                end
                object RefBackupBtn: TButton
                  Left = 330
                  Top = 20
                  Width = 36
                  Height = 22
                  Anchors = [akTop, akRight]
                  Caption = '...'
                  TabOrder = 1
                  OnClick = RefBackupBtnClick
                end
              end
              object DatFilGroupBox: TGroupBox
                Left = 8
                Top = 8
                Width = 375
                Height = 85
                Align = alTop
                Caption = #22522#26412#12487#12540#12479#12501#12449#12452#12523
                TabOrder = 0
                DesignSize = (
                  375
                  85)
                object OptDatFileEdit: TEdit
                  Left = 10
                  Top = 22
                  Width = 319
                  Height = 21
                  Anchors = [akLeft, akTop, akRight]
                  ReadOnly = True
                  TabOrder = 0
                end
                object RefDataBtn: TButton
                  Left = 330
                  Top = 22
                  Width = 36
                  Height = 22
                  Anchors = [akTop, akRight]
                  Caption = '...'
                  TabOrder = 1
                  OnClick = RefDataBtnClick
                end
                object DatSavBtn: TButton
                  Left = 215
                  Top = 53
                  Width = 151
                  Height = 25
                  Action = DatSavAction
                  Anchors = [akTop, akRight]
                  TabOrder = 4
                end
                object SavYearComboBox: TComboBox
                  Left = 144
                  Top = 54
                  Width = 68
                  Height = 21
                  Style = csDropDownList
                  Anchors = [akTop, akRight]
                  TabOrder = 3
                end
                object MergeBtn: TButton
                  Left = 11
                  Top = 53
                  Width = 75
                  Height = 25
                  Caption = #12510#12540#12472'...'
                  TabOrder = 2
                  OnClick = MergeBtnClick
                end
              end
            end
            object Opt2TabSheet: TRzTabSheet
              HelpContext = 10
              Caption = #37197#33394
              Padding.Left = 8
              Padding.Top = 8
              Padding.Right = 8
              Padding.Bottom = 8
              object OptColListBox: TListBox
                Left = 8
                Top = 8
                Width = 269
                Height = 356
                Style = lbOwnerDrawFixed
                Align = alClient
                ItemHeight = 15
                TabOrder = 0
                OnDblClick = RefColBtnClick
                OnDrawItem = OptColListBoxDrawItem
              end
              object RightPanel2: TPanel
                Left = 277
                Top = 8
                Width = 106
                Height = 356
                Align = alRight
                BevelOuter = bvNone
                TabOrder = 1
                object RefColBtn: TButton
                  Left = 39
                  Top = 12
                  Width = 63
                  Height = 22
                  Caption = '...'
                  TabOrder = 1
                  OnClick = RefColBtnClick
                end
                object ExpColBtn: TButton
                  Left = 12
                  Top = 91
                  Width = 90
                  Height = 24
                  Caption = #12456#12463#12473#12509#12540#12488'...'
                  TabOrder = 3
                  OnClick = ExpColBtnClick
                end
                object InpColBtn: TButton
                  Left = 12
                  Top = 61
                  Width = 90
                  Height = 24
                  Caption = #12452#12531#12509#12540#12488'...'
                  TabOrder = 2
                  OnClick = InpColBtnClick
                end
                object SpuitPanel: TPanel
                  Left = 12
                  Top = 12
                  Width = 22
                  Height = 22
                  BevelOuter = bvLowered
                  TabOrder = 0
                  StyleElements = [seBorder]
                  object SpuitImage: TImage
                    Left = 1
                    Top = 1
                    Width = 20
                    Height = 20
                    Align = alClient
                    Center = True
                    Picture.Data = {
                      07544269746D6170F6000000424DF60000000000000076000000280000001000
                      000010000000010004000000000080000000C30E0000C30E0000100000001000
                      000000000000000080000080000000808000800000008000800080800000C0C0
                      C000808080000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
                      FF003333333333333333300333333333333330F033333333333330FF03333333
                      3333330FF033333333333330FF033333333333330FF033333333333330FF0333
                      33333333330FF033333333333330FF0033333333333300003333333333333000
                      0033333333333300000333333333330000033333333333300033333333333333
                      3333}
                    Transparent = True
                    OnMouseDown = SpuitImageMouseDown
                    OnMouseUp = SpuitImageMouseUp
                  end
                end
              end
            end
            object Opt3TabSheet: TRzTabSheet
              HelpContext = 11
              Caption = #12501#12457#12531#12488
              Padding.Left = 8
              Padding.Top = 8
              Padding.Right = 8
              Padding.Bottom = 8
              object BottomPanel3: TPanel
                Left = 303
                Top = 8
                Width = 80
                Height = 356
                Align = alRight
                BevelOuter = bvNone
                TabOrder = 1
                object RefFontBtn: TButton
                  Left = 12
                  Top = 0
                  Width = 60
                  Height = 22
                  Caption = '...'
                  TabOrder = 0
                  OnClick = RefFontBtnClick
                end
              end
              object OptFontListBox: TListBox
                Left = 8
                Top = 8
                Width = 295
                Height = 356
                Style = lbOwnerDrawFixed
                Align = alClient
                ItemHeight = 13
                TabOrder = 0
                OnDblClick = RefFontBtnClick
                OnDrawItem = OptFontListBoxDrawItem
              end
            end
            object Opt4TabSheet: TRzTabSheet
              HelpContext = 12
              Caption = #12362#30693#12425#12379
              Padding.Left = 8
              Padding.Top = 8
              Padding.Right = 8
              Padding.Bottom = 8
              object MsgCndGroupBox: TGroupBox
                Left = 8
                Top = 57
                Width = 375
                Height = 80
                Align = alTop
                Caption = #34920#31034#12377#12427#26465#20214
                TabOrder = 1
                object Chk1CheckBox: TCheckBox
                  Left = 16
                  Top = 24
                  Width = 65
                  Height = 17
                  Caption = #20104#23450'1'
                  TabOrder = 0
                  OnClick = OptionChanged
                end
                object Chk2CheckBox: TCheckBox
                  Left = 88
                  Top = 24
                  Width = 65
                  Height = 17
                  Caption = #20104#23450'2'
                  TabOrder = 1
                  OnClick = OptionChanged
                end
                object Chk3CheckBox: TCheckBox
                  Left = 160
                  Top = 24
                  Width = 65
                  Height = 17
                  Caption = #35352#24565#26085
                  TabOrder = 2
                  OnClick = OptionChanged
                end
                object Chk4CheckBox: TCheckBox
                  Left = 240
                  Top = 24
                  Width = 65
                  Height = 17
                  Caption = #26278#24773#22577
                  TabOrder = 3
                  OnClick = OptionChanged
                end
                object NoChkCheckBox: TCheckBox
                  Left = 16
                  Top = 52
                  Width = 81
                  Height = 17
                  Caption = #28961#26465#20214
                  TabOrder = 4
                  OnClick = OptionChanged
                end
              end
              object TopPanel4: TPanel
                Left = 8
                Top = 8
                Width = 375
                Height = 49
                Align = alTop
                BevelOuter = bvNone
                TabOrder = 0
                object MsgShowCheckBox: TCheckBox
                  Left = 8
                  Top = 16
                  Width = 97
                  Height = 17
                  Caption = #34920#31034#12377#12427
                  TabOrder = 0
                  OnClick = OptionChanged
                end
                object OnceCheckBox: TCheckBox
                  Left = 120
                  Top = 16
                  Width = 105
                  Height = 17
                  Caption = '1'#26085'1'#22238#12398#12415
                  TabOrder = 1
                  OnClick = OptionChanged
                end
              end
              object BottomPanel4: TPanel
                Left = 8
                Top = 137
                Width = 375
                Height = 227
                Align = alClient
                BevelOuter = bvNone
                TabOrder = 2
                DesignSize = (
                  375
                  227)
                object Label4_1: TLabel
                  Left = 8
                  Top = 26
                  Width = 52
                  Height = 13
                  Caption = #12362#30693#12425#12379#38899
                end
                object MsgSoundEdit: TEdit
                  Left = 88
                  Top = 22
                  Width = 249
                  Height = 21
                  Anchors = [akLeft, akTop, akRight]
                  TabOrder = 0
                end
                object RefSndBtn: TButton
                  Left = 339
                  Top = 22
                  Width = 36
                  Height = 22
                  Anchors = [akTop, akRight]
                  Caption = '...'
                  TabOrder = 1
                  OnClick = RefSndBtnClick
                end
                object FitMsgCheckBox: TCheckBox
                  Left = 8
                  Top = 64
                  Width = 241
                  Height = 17
                  Caption = #12459#12524#12531#12480#12540#37096#20998#12395#37325#12397#12390#34920#31034
                  TabOrder = 2
                end
                object TestMsgBtn: TButton
                  Left = 8
                  Top = 122
                  Width = 113
                  Height = 25
                  Caption = #12486#12473#12488#34920#31034
                  TabOrder = 5
                  OnClick = TestMsgBtnClick
                end
                object ModalScrCheckBox: TCheckBox
                  Left = 8
                  Top = 89
                  Width = 225
                  Height = 17
                  Caption = #21322#36879#26126#12473#12463#12522#12540#12531#12391#12513#12452#12531#30011#38754#12434#35206#12358
                  TabOrder = 3
                end
                object ModalAlphaEdit: TLabeledEdit
                  Left = 273
                  Top = 87
                  Width = 42
                  Height = 21
                  Alignment = taRightJustify
                  EditLabel.Width = 36
                  EditLabel.Height = 13
                  EditLabel.Caption = #36879#26126#24230
                  LabelPosition = lpLeft
                  NumbersOnly = True
                  TabOrder = 4
                end
              end
            end
            object Opt5TabSheet: TRzTabSheet
              HelpContext = 13
              Caption = #12381#12398#20182
              Padding.Left = 8
              Padding.Top = 8
              Padding.Right = 8
              Padding.Bottom = 8
              object Label5_3: TLabel
                Left = 25
                Top = 286
                Width = 51
                Height = 13
                Alignment = taRightJustify
                Caption = #36913#12398#22987#12414#12426
              end
              object Label5_4: TLabel
                Left = 202
                Top = 286
                Width = 68
                Height = 13
                Alignment = taRightJustify
                Caption = #26376#12398#24418#12398#26178#21051
              end
              object MarkGroupBox: TGroupBox
                Left = 8
                Top = 8
                Width = 369
                Height = 52
                Caption = #12510#12540#12463
                TabOrder = 0
                object Label5_m1: TLabel
                  Left = 16
                  Top = 24
                  Width = 30
                  Height = 13
                  Caption = #20104#23450'1'
                end
                object Label5_m2: TLabel
                  Left = 112
                  Top = 24
                  Width = 30
                  Height = 13
                  Caption = #20104#23450'2'
                end
                object Label5_m3: TLabel
                  Left = 216
                  Top = 24
                  Width = 36
                  Height = 13
                  Caption = #35352#24565#26085
                end
                object Mark1Edit: TEdit
                  Left = 56
                  Top = 20
                  Width = 40
                  Height = 21
                  MaxLength = 2
                  TabOrder = 0
                  OnChange = OptionChanged
                end
                object Mark2Edit: TEdit
                  Left = 152
                  Top = 20
                  Width = 40
                  Height = 21
                  MaxLength = 2
                  TabOrder = 1
                  OnChange = OptionChanged
                end
                object Mark3Edit: TEdit
                  Left = 264
                  Top = 20
                  Width = 40
                  Height = 21
                  MaxLength = 2
                  TabOrder = 2
                  OnChange = OptionChanged
                end
              end
              object StrWkComboBox: TComboBox
                Left = 78
                Top = 282
                Width = 57
                Height = 21
                Style = csDropDownList
                TabOrder = 4
                OnChange = OptionChanged
              end
              object CountGroupBox: TGroupBox
                Left = 8
                Top = 182
                Width = 369
                Height = 83
                Caption = #12414#12391'('#12363#12425')'#20309#26085
                TabOrder = 3
                object Label5_c1: TLabel
                  Left = 105
                  Top = 24
                  Width = 12
                  Height = 13
                  Caption = #24180
                end
                object Label5_c2: TLabel
                  Left = 161
                  Top = 24
                  Width = 12
                  Height = 13
                  Caption = #26376
                end
                object Label5_c3: TLabel
                  Left = 217
                  Top = 24
                  Width = 12
                  Height = 13
                  Caption = #26085
                end
                object Label5_c4: TLabel
                  Left = 105
                  Top = 56
                  Width = 12
                  Height = 13
                  Caption = #24180
                end
                object Label5_c5: TLabel
                  Left = 161
                  Top = 56
                  Width = 12
                  Height = 13
                  Caption = #26376
                end
                object Label5_c6: TLabel
                  Left = 217
                  Top = 56
                  Width = 12
                  Height = 13
                  Caption = #26085
                end
                object Label5_1: TLabel
                  Left = 8
                  Top = 24
                  Width = 30
                  Height = 13
                  Caption = #25351#23450'1'
                end
                object Label5_2: TLabel
                  Left = 8
                  Top = 56
                  Width = 30
                  Height = 13
                  Caption = #25351#23450'2'
                end
                object CntYear1Edit: TEdit
                  Left = 51
                  Top = 20
                  Width = 50
                  Height = 21
                  MaxLength = 4
                  TabOrder = 0
                  OnChange = OptionChanged
                end
                object CntMon1Edit: TEdit
                  Left = 124
                  Top = 20
                  Width = 33
                  Height = 21
                  MaxLength = 2
                  TabOrder = 1
                  OnChange = OptionChanged
                end
                object CntDay1Edit: TEdit
                  Left = 180
                  Top = 20
                  Width = 33
                  Height = 21
                  MaxLength = 2
                  TabOrder = 2
                  OnChange = OptionChanged
                end
                object CntCurPosCheckBox: TCheckBox
                  Left = 248
                  Top = 24
                  Width = 113
                  Height = 17
                  Caption = #29694#22312#20301#32622#12363#12425' '
                  TabOrder = 3
                  OnClick = OptionChanged
                end
                object CntYear2Edit: TEdit
                  Left = 51
                  Top = 52
                  Width = 50
                  Height = 21
                  MaxLength = 4
                  TabOrder = 4
                  OnChange = OptionChanged
                end
                object CntMon2Edit: TEdit
                  Left = 124
                  Top = 52
                  Width = 33
                  Height = 21
                  MaxLength = 2
                  TabOrder = 5
                  OnChange = OptionChanged
                end
                object CntDay2Edit: TEdit
                  Left = 180
                  Top = 52
                  Width = 33
                  Height = 21
                  MaxLength = 2
                  TabOrder = 6
                  OnChange = OptionChanged
                end
              end
              object MonTimComboBox: TComboBox
                Left = 272
                Top = 282
                Width = 57
                Height = 21
                Style = csDropDownList
                TabOrder = 5
                OnChange = OptionChanged
              end
              object WebTabGroupBox: TGroupBox
                Left = 8
                Top = 66
                Width = 369
                Height = 52
                Caption = #20999#12426#26367#12360#12479#12502
                TabOrder = 1
                object TabStr1Edit: TEdit
                  Left = 16
                  Top = 20
                  Width = 73
                  Height = 21
                  TabOrder = 0
                  OnChange = TabStrEditChange
                end
                object TabStr2Edit: TEdit
                  Tag = 1
                  Left = 103
                  Top = 20
                  Width = 73
                  Height = 21
                  TabOrder = 1
                  OnChange = TabStrEditChange
                end
                object TabStr3Edit: TEdit
                  Tag = 2
                  Left = 191
                  Top = 20
                  Width = 73
                  Height = 21
                  TabOrder = 2
                  OnChange = TabStrEditChange
                end
                object TabStr4Edit: TEdit
                  Tag = 3
                  Left = 279
                  Top = 20
                  Width = 73
                  Height = 21
                  TabOrder = 3
                  OnChange = TabStrEditChange
                end
              end
              object IMECheckBox: TCheckBox
                Left = 263
                Top = 338
                Width = 127
                Height = 17
                Caption = #20837#21147#27396#12391'IME'#12458#12531
                TabOrder = 8
              end
              object KoyomiCheckBox: TCheckBox
                Left = 12
                Top = 338
                Width = 246
                Height = 17
                Caption = #26087#26278#12289#24178#25903#12289#20845#26332#12289#31680#27671#12539#38609#31680#12434#34920#31034
                TabOrder = 7
                OnClick = OptionChanged
              end
              object TabStyleGroupBox: TGroupBox
                Left = 8
                Top = 124
                Width = 369
                Height = 52
                Caption = #12479#12502#12398#34920#31034#12473#12479#12452#12523
                TabOrder = 2
                object Label1: TLabel
                  Left = 18
                  Top = 23
                  Width = 47
                  Height = 13
                  Alignment = taRightJustify
                  Caption = #12506#12540#12472#12479#12502
                end
                object Label2: TLabel
                  Left = 197
                  Top = 23
                  Width = 58
                  Height = 13
                  Alignment = taRightJustify
                  Caption = #20999#12426#26367#12360#12479#12502
                end
                object TabStyle1ComboBox: TComboBox
                  Left = 69
                  Top = 20
                  Width = 100
                  Height = 22
                  Style = csOwnerDrawFixed
                  TabOrder = 0
                  OnChange = TabStyle1ComboBoxChange
                  OnDrawItem = TabStyleComboBoxDrawItem
                end
                object TabStyle2ComboBox: TComboBox
                  Tag = 1
                  Left = 259
                  Top = 20
                  Width = 100
                  Height = 22
                  Style = csOwnerDrawFixed
                  TabOrder = 1
                  OnChange = TabStyle2ComboBoxChange
                  OnDrawItem = TabStyleComboBoxDrawItem
                end
              end
              object SttMoonInfCheckBox: TCheckBox
                Left = 12
                Top = 315
                Width = 357
                Height = 17
                Caption = #12473#12486#12540#12479#12473#12496#12540#12395#36984#25246#26085#20184#12398#26376#40802#24773#22577#12434#34920#31034
                TabOrder = 6
                OnClick = OptionChanged
              end
            end
            object Opt6TabSheet: TRzTabSheet
              HelpContext = 14
              Caption = #12471#12519#12540#12488#12459#12483#12488
              Padding.Left = 8
              Padding.Top = 8
              Padding.Right = 8
              Padding.Bottom = 8
              object RightPanel6: TPanel
                Left = 287
                Top = 8
                Width = 96
                Height = 356
                Align = alRight
                BevelOuter = bvNone
                TabOrder = 0
                object CtrlCheckBox: TCheckBox
                  Left = 16
                  Top = 67
                  Width = 65
                  Height = 17
                  Caption = 'Ctrl'
                  TabOrder = 1
                end
                object ShiftCheckBox: TCheckBox
                  Left = 16
                  Top = 91
                  Width = 65
                  Height = 17
                  Caption = 'Shift'
                  TabOrder = 2
                end
                object SKChgBtn: TButton
                  Left = 12
                  Top = 208
                  Width = 73
                  Height = 25
                  Caption = #22793#26356
                  TabOrder = 4
                  OnClick = SKChgBtnClick
                end
                object ClrBtn: TButton
                  Left = 12
                  Top = 246
                  Width = 73
                  Height = 25
                  Caption = #35299#38500
                  TabOrder = 5
                  OnClick = ClrBtnClick
                end
                object AltCheckBox: TCheckBox
                  Left = 16
                  Top = 115
                  Width = 65
                  Height = 17
                  Caption = 'Alt'
                  TabOrder = 3
                end
                object KeyComboBox: TComboBox
                  Left = 10
                  Top = 23
                  Width = 81
                  Height = 21
                  Style = csDropDownList
                  DropDownCount = 20
                  ImeMode = imClose
                  TabOrder = 0
                end
              end
              object ListPanel6: TPanel
                Left = 8
                Top = 8
                Width = 279
                Height = 356
                Align = alClient
                BevelOuter = bvNone
                TabOrder = 1
                object CategoryComboBox: TComboBox
                  Left = 0
                  Top = 0
                  Width = 279
                  Height = 21
                  Align = alTop
                  Style = csDropDownList
                  TabOrder = 0
                  OnChange = CategoryComboBoxChange
                end
                object ShortCutList: TListBox
                  Left = 0
                  Top = 21
                  Width = 279
                  Height = 335
                  Align = alClient
                  ItemHeight = 13
                  TabOrder = 1
                  TabWidth = 120
                  OnClick = ShortCutListClick
                end
              end
            end
            object VersionTabSheet: TRzTabSheet
              HelpContext = 15
              Caption = #12496#12540#12472#12519#12531#24773#22577
              object VersionPanel: TPanel
                Left = 12
                Top = 12
                Width = 280
                Height = 180
                BevelOuter = bvLowered
                Color = clNavy
                Padding.Top = 16
                TabOrder = 0
                StyleElements = [seBorder]
                object VersionLabel: TLabel
                  Left = 13
                  Top = 110
                  Width = 255
                  Height = 15
                  Alignment = taCenter
                  AutoSize = False
                  Caption = 'Version'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWhite
                  Font.Height = -13
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                end
                object UrlLabel: TLabel
                  Left = 13
                  Top = 140
                  Width = 255
                  Height = 15
                  Cursor = crHandPoint
                  Alignment = taCenter
                  AutoSize = False
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clAqua
                  Font.Height = -13
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  OnClick = UrlLabelClick
                end
                object TitleLabel: TLabel
                  Left = 1
                  Top = 17
                  Width = 278
                  Height = 27
                  Align = alTop
                  Alignment = taCenter
                  AutoSize = False
                  Caption = #26376#12513#12514
                  Font.Charset = SHIFTJIS_CHARSET
                  Font.Color = clWhite
                  Font.Height = -27
                  Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
                  Font.Style = [fsBold]
                  ParentFont = False
                end
                object IconImage: TImage
                  Left = 128
                  Top = 56
                  Width = 32
                  Height = 32
                  AutoSize = True
                  Picture.Data = {
                    07544269746D6170360C0000424D360C00000000000036000000280000002000
                    0000200000000100180000000000000C0000130B0000130B0000000000000000
                    0000848242848242848242848242848242848242848242848242848242848242
                    84824284824284824200000000000000000000828400FFFF00FFFF00FFFF0082
                    8484824284824284824284824284824284824284824284824284824284824284
                    8242848242848242848242848242848242848242848242848242848242848242
                    00000000000000000000000000000000000000000000000000000000000000FF
                    FF00FFFF00FFFF84824284824284824284824284824284824284824284824284
                    8242848242848242848242848242848242848242848242000000000000000000
                    0000000000000000000000000000000000000000000000000000000000000000
                    0000000000FFFF00FFFF00FFFF84824284824284824284824284824284824284
                    8242848242848242848242848242848242848242000000000000000000000000
                    0000000000000000000000000000000000000000000000000000000000000000
                    0000000000000000FFFF00FFFF00FFFF84824284824284824284824284824284
                    8242848242848242848242848242848242000000000000000000000000000000
                    0000000000000000000000000000000000000000000000000000000000000000
                    0000000000000000000000FFFF00FFFF00FFFF84824284824284824284824284
                    8242848242848242848242848242000000000000000000000000000000000000
                    0000000000000000000000000000000000000000000000000000000000000000
                    0000000000000000000000000000FFFF00FFFF00FFFF84824284824284824284
                    8242848242848242848242000000000000000000000000000000000000000000
                    0000000000000000000000000000000000000000000000000000000000000000
                    0000000000000000000000000000000000FFFF00FFFF00FFFF84824284824284
                    8242848242848242848242000000000000000000000000000000000000000000
                    0000000000000000000000000000000000000000000000000000000000000000
                    0000000000000000000000000000000000000000FFFF00FFFF00FFFF84824284
                    8242848242848242000000000000848284848284848284848284848284848284
                    8482848482848482848482848482848482848482848482848482848482848482
                    8484828484828484828484828484828400000000FFFF00FFFF00FFFF84824284
                    8242848242848242000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFF84828400000000828400FFFF00FFFF00FFFF84
                    8242848242848242000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                    FFFFFFFFFFFFFFFFFFFFFFFFFF84828400000000000000FFFF00FFFF00FFFF84
                    8242848242000000000000000000000000000000000000000000000000000000
                    0000000000000000000000000000000000000000000000000000000000000000
                    00000000000000000000FFFFFF84828400000000000000FFFF00FFFF00FFFF84
                    8242848242C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6
                    C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3
                    C6C6C3C6C6C3C6000000FFFFFF84828400000000000000FFFF00FFFF00FFFF00
                    FFFF848242C6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFF
                    C6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6848284848284C6C3C68482
                    84848284C6C3C6000000FFFFFF84828400000000000000FFFF00FFFF00FFFF00
                    FFFF848242C6C3C6FFFFFF0000FFC6C3C6FFFFFF840000C6C3C6FFFFFF840000
                    C6C3C6FFFFFF840000C6C3C6FFFFFF840000C6C3C6848284848284C6C3C68482
                    84848284C6C3C6000000FFFFFF84828400000000000000FFFF00FFFF00FFFF00
                    FFFF848242C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6
                    C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3
                    C6C6C3C6C6C3C6000000FFFFFF84828400000000000000FFFF00FFFF00FFFF00
                    FFFF848242C6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFF
                    C6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFF
                    FFFFFFFFC6C3C6000000FFFFFF84828400000000000000FFFF00FFFF00FFFF00
                    FFFF848242C6C3C6FFFFFF0000FFC6C3C6FFFFFF840000C6C3C6FFFFFF840000
                    C6C3C6FFFFFF840000C6C3C6FFFFFF840000C6C3C6FFFFFF840000C6C3C6FFFF
                    FFFF0000C6C3C6000000FFFFFF84828400000000000000FFFF00FFFF00FFFF00
                    FFFF848242C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6
                    C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3
                    C6C6C3C6C6C3C6000000FFFFFF84828400000000000000FFFF00FFFF00FFFF00
                    FFFF848242C6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFF
                    C6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFF
                    FFFFFFFFC6C3C6000000FFFFFF84828400000000000000FFFF00FFFF00FFFF00
                    FFFF848242C6C3C6FFFFFF0000FFC6C3C6FFFFFF840000C6C3C6FFFFFF840000
                    C6C3C6FFFFFF840000C6C3C6FFFFFF840000C6C3C6FFFFFF840000C6C3C6FFFF
                    FFFF0000C6C3C6000000FFFFFF84828400000000828400FFFF00FFFF00FFFF84
                    8242848242C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6
                    C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3
                    C6C6C3C6C6C3C6000000FFFFFF84828400000000FFFF00FFFF00FFFF00FFFF84
                    8242848242C6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFF
                    C6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFF
                    FFFFFFFFC6C3C6000000FFFFFF84828400000000FFFF00FFFF00FFFF00FFFF84
                    8242848242C6C3C6FFFFFF0000FFC6C3C6FFFFFF840000C6C3C6FFFFFF840000
                    C6C3C6FFFFFF840000C6C3C6FFFFFF840000C6C3C6FFFFFF840000C6C3C6FFFF
                    FFFF0000C6C3C6000000FFFFFF84828400000000FFFF00FFFF00FFFF84824284
                    8242848242C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6
                    C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3
                    C6C6C3C6C6C3C6000000FFFFFF00000000FFFF00FFFF00FFFF00FFFF84824284
                    8242848242C6C3C6848284848284C6C3C6848284848284C6C3C6C6C3C6C6C3C6
                    C6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFFFFFFFFFFC6C3C6FFFF
                    FFFFFFFFC6C3C600000000000000000000FFFF00FFFF00FFFF84824284824284
                    8242848242C6C3C6848284848284C6C3C6848284848284C6C3C6848284C6C3C6
                    C6C3C6FFFFFF840000C6C3C6FFFFFF840000C6C3C6FFFFFF840000C6C3C6FFFF
                    FFFF0000C6C3C600000000000000FFFF00FFFF00FFFF84824284824284824284
                    8242848242C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6
                    C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3C6C6C3
                    C6C6C3C6C6C3C600000000FFFF00FFFF00FFFF84824284824284824284824284
                    8242848242848242848242848242848242848242000000000000000000000000
                    0000000000000000000000000000000000000000000000000000000000000000
                    0000000000000000FFFF00FFFF00FFFF84824284824284824284824284824284
                    8242848242848242848242848242848242848242848242848242000000000000
                    0000000000000000000000000000000000000000000000000000000000000000
                    0000000000FFFF00FFFF00FFFF84824284824284824284824284824284824284
                    8242848242848242848242848242848242848242848242848242848242848242
                    00000000000000000000000000000000000000000000000000000000828400FF
                    FF00FFFF00FFFF84824284824284824284824284824284824284824284824284
                    8242848242848242848242848242848242848242848242848242848242848242
                    84824284824284824200000000000000000000828400FFFF00FFFF00FFFF0082
                    8484824284824284824284824284824284824284824284824284824284824284
                    8242}
                  Transparent = True
                end
              end
            end
          end
        end
      end
    end
  end
  object RzStatusBar1: TRzStatusBar
    Left = 0
    Top = 452
    Width = 663
    Height = 19
    BorderInner = fsNone
    BorderOuter = fsNone
    BorderSides = [sdLeft, sdTop, sdRight, sdBottom]
    BorderWidth = 0
    Color = 15987699
    GradientColorStyle = gcsCustom
    TabOrder = 2
    VisualStyle = vsGradient
    object RzStatusPane1: TRzStatusPane
      Left = 0
      Top = 0
      Height = 19
      Align = alLeft
      Caption = ''
    end
    object RzStatusPane2: TRzStatusPane
      Left = 100
      Top = 0
      Height = 19
      Align = alLeft
      Alignment = taRightJustify
      Caption = ''
    end
    object RzStatusPane3: TRzStatusPane
      Left = 200
      Top = 0
      Width = 463
      Height = 19
      Align = alClient
      OnDblClick = RzStatusPane3DblClick
      Caption = ''
    end
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 168
    Top = 264
  end
  object ActionList1: TActionList
    Left = 49
    Top = 16
    object TodayAction: TAction
      Category = #31227#21205
      HelpType = htContext
      Hint = #20170#26085#12398#20301#32622
      OnExecute = MoveBtnClick
    end
    object NxtDayAction: TAction
      Tag = 1
      Category = #31227#21205
      HelpType = htContext
      Hint = #27425#12398#26085#12395#31227#21205
      OnExecute = MoveBtnClick
    end
    object BefDayAction: TAction
      Tag = -1
      Category = #31227#21205
      HelpType = htContext
      Hint = #21069#12398#26085#12395#31227#21205
      OnExecute = MoveBtnClick
    end
    object NxtWekAction: TAction
      Tag = 2
      Category = #31227#21205
      HelpType = htContext
      Hint = #27425#12398#36913#12395#31227#21205
      OnExecute = MoveBtnClick
    end
    object BefWekAction: TAction
      Tag = -2
      Category = #31227#21205
      HelpType = htContext
      Hint = #21069#12398#36913#12395#31227#21205
      OnExecute = MoveBtnClick
    end
    object NxtMonAction: TAction
      Tag = 3
      Category = #31227#21205
      HelpType = htContext
      Hint = #27425#12398#26376#12395#31227#21205
      OnExecute = MoveBtnClick
    end
    object BefMonAction: TAction
      Tag = -3
      Category = #31227#21205
      HelpType = htContext
      Hint = #21069#12398#26376#12395#31227#21205
      OnExecute = MoveBtnClick
    end
    object NxtYearAction: TAction
      Tag = 4
      Category = #31227#21205
      HelpContext = 4
      HelpType = htContext
      Hint = #27425#12398#24180#12395#31227#21205
      OnExecute = MoveBtnClick
    end
    object BefYearAction: TAction
      Tag = -4
      Category = #31227#21205
      HelpContext = 4
      HelpType = htContext
      Hint = #21069#12398#24180#12395#31227#21205
      OnExecute = MoveBtnClick
    end
    object NxtFulAction: TAction
      Category = #31227#21205
      Caption = #27425#12398#28288#26376' (&F)'
      HelpType = htContext
      Hint = #27425#12398#28288#26376#12395#31227#21205
      OnExecute = NxtFulActionExecute
    end
    object NxtNewAction: TAction
      Category = #31227#21205
      Caption = #27425#12398#26032#26376' (&N)'
      HelpType = htContext
      Hint = #27425#12398#26032#26376#12395#31227#21205
      OnExecute = NxtNewActionExecute
    end
    object InpDateAction: TAction
      Category = #31227#21205
      Caption = #25351#23450#12375#12383#26085#20184#12408'(&J) ...'
      HelpType = htContext
      Hint = #25351#23450#12375#12383#26085#20184#12408
      OnExecute = InpDateActionExecute
    end
    object CntDay1Action: TAction
      Category = #31227#21205
      Caption = #12414#12391'('#12363#12425')'#20309#26085#12398#26085#20184'1'#12395#31227#21205
      HelpType = htContext
      Hint = #12414#12391'('#12363#12425')'#20309#26085#12398#25351#23450#26085'1'#12408
      OnExecute = ChgDateExecute
    end
    object CntDay2Action: TAction
      Category = #31227#21205
      Caption = #12414#12391'('#12363#12425')'#20309#26085#12398#26085#20184'2'#12395#31227#21205
      HelpType = htContext
      Hint = #12414#12391'('#12363#12425')'#20309#26085#12398#25351#23450#26085'2'#12408
      OnExecute = ChgDateExecute
    end
    object ChgMemo1Action: TAction
      Category = #20999#12426#25563#12360
      Caption = #20104#23450'1'
      HelpType = htContext
      Hint = #20104#23450'1'#12395#20999#12426#25563#12360
      OnExecute = ChgMemoActionExecute
    end
    object ChgMemo2Action: TAction
      Tag = 1
      Category = #20999#12426#25563#12360
      Caption = #20104#23450'2'
      HelpType = htContext
      Hint = #20104#23450'2'#12395#20999#12426#25563#12360
      OnExecute = ChgMemoActionExecute
    end
    object ChgMemo3Action: TAction
      Tag = 2
      Category = #20999#12426#25563#12360
      Caption = 'ChgMemo3Action'
      HelpType = htContext
      Hint = #35352#24565#26085#12395#20999#12426#25563#12360
      OnExecute = ChgMemoActionExecute
    end
    object ChgMemo4Action: TAction
      Tag = 3
      Category = #20999#12426#25563#12360
      Caption = #26278#24773#22577
      HelpType = htContext
      Hint = #26278#24773#22577#12395#20999#12426#25563#12360
      OnExecute = ChgMemoActionExecute
    end
    object ChgWeekAction: TAction
      Category = #20999#12426#25563#12360
      Caption = #36913#38291#20104#23450
      HelpType = htContext
      Hint = #36913#38291#20104#23450#30011#38754#12395#20999#12426#25563#12360
      OnExecute = ChgTabActionExecute
    end
    object ChgMemoAction: TAction
      Tag = 10
      Category = #20999#12426#25563#12360
      Caption = #12513#12514'1'
      HelpType = htContext
      Hint = #12513#12514#30011#38754'1'#12395#20999#12426#25563#12360
      OnExecute = ChgTabActionExecute
    end
    object ChgMemoYAction: TAction
      Tag = 11
      Category = #20999#12426#25563#12360
      Caption = 'ChgMemoYAction'
      HelpType = htContext
      Hint = #12513#12514#30011#38754'2'#12395#20999#12426#25563#12360
      OnExecute = ChgTabActionExecute
    end
    object ChgMemoZAction: TAction
      Tag = 12
      Category = #20999#12426#25563#12360
      Caption = 'ChgMemoZAction'
      HelpType = htContext
      Hint = #12513#12514#30011#38754'3'#12395#20999#12426#25563#12360
      OnExecute = ChgTabActionExecute
    end
    object ChgFindAction: TAction
      Tag = 20
      Category = #20999#12426#25563#12360
      Caption = 'ChgFindAction'
      HelpType = htContext
      Hint = #26908#32034#30011#38754#12395#20999#12426#25563#12360
      OnExecute = ChgTabActionExecute
    end
    object TodayMsgAction: TAction
      Category = #12381#12398#20182
      Caption = #26412#26085#12398#12362#30693#12425#12379
      HelpType = htContext
      Hint = #26412#26085#12398#12362#30693#12425#12379#12434#34920#31034
      OnExecute = TodayMsgActionExecute
    end
    object CloseAction: TAction
      Category = #12381#12398#20182
      Caption = #32066#20102
      HelpType = htContext
      Hint = #26376#12513#12514#12434#32066#20102
      OnExecute = CloseActionExecute
    end
    object HelpAction: TAction
      Category = #12381#12398#20182
      HelpType = htContext
      Hint = #12504#12523#12503
      OnExecute = HelpActionExecute
    end
    object StartFindAction: TAction
      Category = #20869#37096
      Caption = #38283#22987
      HelpType = htContext
      Hint = #26908#32034#38283#22987
      OnExecute = StartFindActionExecute
      OnUpdate = StartFindActionUpdate
    end
    object DatSavAction: TAction
      Category = #20869#37096
      Caption = #24180#20197#21069#12398#12487#12540#12479#12434#25972#29702
      HelpType = htContext
      OnExecute = DatSavExecute
      OnUpdate = DatSavActionUpdate
    end
    object FindCopySelAction: TAction
      Category = #20869#37096
      Caption = #36984#25246#38917#30446#12434#12467#12500#12540'(&C)'
      OnExecute = FindCopySelActionExecute
      OnUpdate = FindCopySelActionUpdate
    end
    object FindCopyAllAction: TAction
      Category = #20869#37096
      Caption = #20840#12390#12398#38917#30446#12434#12467#12500#12540'(&A)'
      OnExecute = FindCopyAllActionExecute
      OnUpdate = FindCopyAllActionUpdate
    end
  end
  object ColorDialog1: TColorDialog
    Left = 120
    Top = 16
  end
  object FontDialog1: TFontDialog
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
    Font.Style = []
    Left = 184
    Top = 16
  end
  object OpenDialog1: TOpenDialog
    Left = 166
    Top = 112
  end
  object SaveDialog1: TSaveDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 166
    Top = 160
  end
  object JumpPopupMenu: TPopupMenu
    AutoHotkeys = maManual
    HelpContext = 4
    OnPopup = JumpPopupMenuPopup
    Left = 48
    Top = 112
  end
  object CalPopupMenu: TPopupMenu
    AutoHotkeys = maManual
    HelpContext = 4
    OnPopup = CalPopupMenuPopup
    Left = 48
    Top = 176
    object NxtFulMonItem: TMenuItem
      Action = NxtFulAction
    end
    object NxtNewMonItem: TMenuItem
      Action = NxtNewAction
    end
    object CalPop_s1: TMenuItem
      Caption = '-'
    end
    object MovCntDay1Item: TMenuItem
      Action = CntDay1Action
    end
    object MovCntDay2Item: TMenuItem
      Action = CntDay2Action
    end
    object TodayItem: TMenuItem
      Action = TodayAction
    end
    object CalPop_s2: TMenuItem
      Caption = '-'
    end
    object MovInpDayItem: TMenuItem
      Action = InpDateAction
    end
  end
  object FindPopupMenu: TPopupMenu
    AutoHotkeys = maManual
    HelpContext = 7
    Left = 48
    Top = 232
    object FindCopySelItem: TMenuItem
      Action = FindCopySelAction
    end
    object FindCopyAllItem: TMenuItem
      Action = FindCopyAllAction
    end
  end
  object ApplicationEvents1: TApplicationEvents
    OnMessage = ApplicationEvents1Message
    Left = 46
    Top = 307
  end
  object SpuitTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = SpuitTimerTimer
    Left = 168
    Top = 312
  end
end
