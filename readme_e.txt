----------------------------------------------------------------------
FF7FC (Final Fantasy VII with NES quality graphic/sound)
----------------------------------------------------------------------
* Programmer
  BETA

* Program Operation
  - U/D/L/R             Arrow keys
  - OK                  x
  - CANCEL              z
  - MENU                space
  - Switch character    c
  - Exit program        ESC or q
  - Forced encounter    s
  - Show debug info     a
  - Toggle encounter    a + s
  - Output event info   F1
  - Speed up            Numeric Keypad +
  - Speed down          Numeric Keypad -

* Configuration
  - config_CS.exe
    > Change screen size
    > V-sync on/off
    > Select language
        Japanese
        English
    > Program start phase
        FF3LikeTextEvent: Text intro like FF3
        TitleScreen:      Main title
        StartOrContinue:  "new game" "continue" menu
    > Initial map ID and character coordination
    > Program behavior on losing window focus
        Whether to continue background or suspend

  - keyConfig.exe
    > Change key assignment
        Up/Down/Left/Right
        A (OK)
        B (CANCEL)
        select (MENU)
        start (switch character)
        dbg1
        dbg2

* Environment
  Normal operation confirmed on the following environment:
  - OS      windows7
  - CPU     Intel Corei5 CPU 750 @2.67GHz
  - MEMORY  4.00GB
  - VIDEO   NVIDIA GeForce GT220

* Folder contents
  FF7FC
  |--documents/             Various documents
  |                         (Japanese documents only for the time being)
  |
  |--event/                 Event definition files
  |  |--script/             Event scripts (Japanese)
  |  |--script_e/           Event scripts (English)
  |  |--event.csv           Event description (Japanese)
  |  `--event_e.csv         Event description (English)
  |
  |--image/                 Image files
  |
  |--item/                  Item data (Japanese)
  |
  |--item_e/                Item data (English)
  |
  |--mapdata/               Map data folder
  |  |-- mapdata_000        Map data created with the map editor
  |  |        |
  |  |-- mapdata_999
  |  `-- setting.txt        Defines the following information for each map:
  |                         - BGM file name
  |                         - Battle background image ID
  |                         - Encounterable monster group ID
  |
  |--monsterdata/           Monster data files (Japanese)
  |  |-- monster_data.csv        Defines monsters' status, image, etc
  |  `-- monster_party_data.txt  Defines possible enemy combinations
  |
  |--monsterdata_e/         Monster data files (English)
  |  |-- monster_data.csv        Defines monsters' status, image, etc
  |  `-- monster_party_data.txt  Defines possible enemy combinations
  |
  |--save/                  Game save files
  |
  |--sound/                 Sound files
  |  |--bgm/                BGM files
  |  |--se/                 SE files
  |  `--setting.txt         Defines BGM file names (non-battle)
  |
  |--text/                  Text resources (Japanese)
  |--text_e/                Text resources (English)
  |  |--make_map_data/      Map data generate tool
  |  |--map_chip_edit/      Unknown (old program?)
  |  `--map_chip_id_replacer/   Map chip ID converter
  |
  |--config_CS.exe          Runtime config setup tool
  |--FF7FC.exe              Main game program
  |--ini.bin                Runtime config data
  |--keyConfig.exe          Key assignment setup tool
  |--keySetting.bin         Key assignment config data
  |--readme.txt             readme (Japanese)
  |--readme_e.txt           This file
  |--SaveDataEditor.exe     Game save data tweak tool

* Setup BGM

Edit "./mapdata/setting.txt" to change BGM for a specific map.
Each line defines the following info for map ID 0 to 999.

    id              : map ID
    type            : reserved
    bgm             : BGM file name
    battle_back     : Battle background image ID
    monster_party_id: Encounterable monster party ID

Modify the BGM file name field for the corresponding map.

