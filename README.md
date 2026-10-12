# MMRecompRando
This is a randomizer mod for Majora's Mask: Recompiled with both singleplayer and Archipelago multiworld support.

## Disclaimer
This randomizer is still under development in the beta phase but is considered stable for playing. You may encounter features that are not finished yet or errors. Be sure to check the Issues page and report anything you encounter there.

## Setup
### Prerequesites
In order to be able to play the randomizer, you'll first need the following:
- Download [Zelda64Recomp](https://github.com/Zelda64Recomp/Zelda64Recomp/releases/latest) and unzip its contents anywhere except in any folder related to OneDrive, such as `Documents` or `Pictures`;
- Downlaod [Visual Studio C++ Redist](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version);
- A legal ROM of The Legend of Zelda: Majora's Mask - US.

### Mod Installation
1. Download the mod: [MMRecompRando.zip](https://github.com/RecompRando/MMRecompRando/releases/latest);
2. Launch `Zelda 64: Recompiled`;
3. Install the mod using one of the following methods:
    1. Click on `Mods` to open the mod menu, click `Install Mods`, then select `MMRecompRando.zip`;
    2. Click and drag the `MMRecompRando.zip` file on top of the `Zelda 64: Recompiled` window.
> [!IMPORTANT]
> Do not unzip the `MMRecompRando.zip` file.

### Optional Setup
- [Thunderstore](https://thunderstore.io/c/zelda-64-recompiled/) to download additional mods or texture packs;
  - The installation process for any mod/texture pack is the same as instructed above in the [Mod Installation](#mod-installation) section.
- [PopTracker](https://github.com/black-sliver/PopTracker/releases/latest) along with the [pack](https://github.com/G4M3RL1F3/Majoras-Mask-AP-PopTracker-Pack/releases/latest) for auto-tracking.

### Solo Setup
1. With the `MMRecompRando` mod enabled, click `Start Game`;
2. Select `Solo` in the randomizer menu;
3. Create a new session and choose the settings you'd like to play with;
4. Click `Generate` and select your seed.

### Multiworld Setup
#### YAML Creation
Every player wishing to play Majora's Mask: Recompiled Randomizer requires a YAML file. A YAML file is the file used to configure each player's world. To obtain and configure such a file, you must follow these steps:
1. Download and install the [Archipelago software](https://github.com/ArchipelagoMW/Archipelago/releases/latest) (install directory defaults to `C:\ProgramData\Archipelago`);
2. Download `mm_recomp.apworld` found in the [latest release page](https://github.com/RecompRando/MMRecompRando/releases/latest);
3. Move the `mm_recomp.apworld` file in `Archipelago\custom_worlds`;
4. Launch the Archipelago launcher;
5. Follow one of these two steps:
    1. Search for and click `Generate Template Options`. This will open a window with YAMLs of every world installed (if not, they can be found in `Archipelago\Players\Templates`). Search for and open `Majora's Mask Recompiled.yaml` and edit your options as desired with the use of a text editor, such as Notepad++;
    2. Search for and click `Options Creator` and edit your options. Once it's done, click the `Export Options` and save it somewhere.

#### Seed Creation
Only the host needs to follow the following steps in order to create a seed:
1. Collect each player's YAML files and move them all to `Archipelago\Players`;
    - If there are players playing other unsupported games (games that are not found on the Archipelago website), make sure to collect the corresponding APWorlds and move them to `Archipelago\custom_worlds`. Refer to the games' Discord channels on the Archipelago server on how to obtain those APWorlds.
2. Launch the Archipelago launcher;
3. Search for and click `Generate`. If the generation was successful, the output `.zip` file is in `Archipelago\output`;
4. Go to the [Archipelago](https://archipelago.gg/) website, click on `Get Started` → `Host Game`, then upload the `.zip` file you generated in the previous step.

#### Connecting to a Multiworld
1. Launch `Zelda 64: Recompiled` and make sure the randomizer mod is enabled;
2. Click `Start Game`;
3. Select `Multiworld` in the randomizer menu;
3. Input the Server Address (usually `archipelago.gg`), your Player/Slot Name, and Password (if applicable);
4. Click `Connect`.
    - If Zelda 64: Recompiled crashes, there is a mismatch between the version of the MMRecompRando mod and the APWorld used to generate the multiworld. Make sure to update your mod and the host to update the APWorld.
At this point, you are ready to play the randomizer.

## Changes from Vanilla
- Scarecrow song is free. Pulling out the ocarina will spawn the scarecrow without needing to play any note;
- Razor Sword does not degrade;
- Blast Mask has no cooldown;
- Bremen Mask makes Link move faster when marching;
- Fierce Deity Mask can be used anywhere;
- Both Archery minigames give both rewards when reaching the highest score;
- The Takkuri in Termina Field does not steal items;
- Kotake gives free red potion refills when speaking to her;
- The goron race in Goron Racetrack only needs to be completed once during the seed. Speaking to the baby goron after completing the race will give gold dust refill.
- When Mask of Truth is acquired, the selected dog for the Doggy Racetrack will run significantly faster than all others. It does not need to be equipped;
- Like Likes do not eat the Hero's Shield;
- The New Wave Bossa Nova location ("Great Bay Baby Zora Song") requires only one egg instead of seven;
- In Ikana Graveyard night 3 grave, all patches are guaranteed to have a flame. This removes the requirement to use one of the lifts to spawn the Big Poe;
- The child on the moon that wears the Majora's Mask can send you back to day 1 if you don't have the requirements to enter Majora's Lair.

## FAQ
### I launched this mod and my save is gone? Did rando delete my save?
Don't worry, your vanilla saves are intact. When playing this mod, a per-session savefile will be created. Your normal save file will not be touched.

### I got a trade item that overwrote another one I had. Is it just gone?
It is not gone, you still have it. Open the inventory menu, move your cursor to that slot and press the `A` or `L` button. It will cycle through all available trade items in that slot.

### I turned on "Intro Checks", but the door to go back through Clock Tower is closed. How am I supposed to go through?
Approach the door and press the `A` button. It will open the door.

### I approached a Great Fairy fountain, but nothing happened. Am I not supposed to get a reward from it?
If nothing happened, then "Shuffle Great Fairy Rewards" is set to "Disabled". These locations are not shuffled in the pool. Therefore, you do not need to do them.

### I hinted for "Bottle of Gold Dust", but it doesn't seem to exists. Where is it supposed to be?
Gold Dust is not an item you will find in the item pool. You will find it at Goron Racetrack after speaking to the baby goron once the race is complete.

### Why can't I buy Magic Beans?
To ensure players don't go out of logic, finding the Magic Beans item in the pool is required. Only then you will be able to buy more from the swamp Scrub Merchant and the bean seller in the Deku Palace grotto.

### Logic
#### The logic expects me to reach the "Twin Islands Ramp Grotto", but I don't have Goron Mask.
There is a scarecrow that can be spawned at the top of the ramp. Stand on the first island and pull out the ocarina. You will then be able to hookshot the scarecrow.

#### The logic expects me to open the "Romani Ranch Doggy Racetrack Rooftop Chest", but I don't have access to Magic Beans and don't have Hookshot.
You can transform into Zora to climb on the fence, then wall-climb onto the rooftop.

#### The logic wants me to get the three Gold Skulltulas in "Ocean Spider House Chandelier Room", but I can't bonk the table without Goron Mask.
Human form can bonk the table by simply rolling into it.

#### The logic expects me to reach Ikana Canyon, but I do not have the Garo Mask.
The hookshotable tree in Road to Ikana can also be spawned with the Gibdo Mask. This is vanilla behavior.

#### This location requires [insert item name here], but it is very easy to get without it.
Some locations are indeed easy to get without the logically required items, but not everyone knows about everything. There will have logic tricks in the future that will take those into account.

#### This location is out of logic, but is very easily obtainable with the Fierce Deity Mask.
Any movement specific to Fierce Deity form is not accounted for logic yet. It will be in the future.

### I opened a chest/got a check and it always shows a grey Archipelago item, what's wrong with you?
The phantom AP item means either something went wrong with your connection or the check is not yet implemented. Please use a text client or the PopTracker pack (see [Optional Setup](#optional-setup)) to see checks that you can definitely get.

If you picked up a phantom on a check that should be implemented, you can open an issue on this repo letting us know!

## Known Issues
- Kotake sometimes does not show the price of her shopsanity item in the second dialogue.
- When using mod version 0.9.5, the game will crash when connecting to a seed generated on version 0.9.4 or below. Until this is properly addressed, a temporary solution is to downgrade the mod to [0.9.4](https://github.com/RecompRando/MMRecompRando/releases/tag/v0.9.4).

## Credit for additional mods included
Thank you to the original authors of some of the other recomp mods used!

- ThatHypedPerson (Color Mods)
- danielryb (Better SoDT, Z-Target Fix, Song Utils)

## Contributing
> [!WARNING]
> The developers and maintainers of this randomizer, along with the N64 Recompiled community as a whole, disallow any "vibe-coded" contributions to this repository. Any contributions or extra features for this randomizer which are primarily AI generated are not endorsed by us.
>
> For more information see [CONTRIBUTING.md](CONTRIBUTING.md).

## Writing mods
See [this document](https://hackmd.io/fMDiGEJ9TBSjomuZZOgzNg) for an explanation of the modding framework, including how to write function patches and perform interop between different mods.

## Tools
You'll need to install `clang` and `make` to build rando.
* On Windows, using [chocolatey](https://chocolatey.org/) to install both is recommended. The packages are `llvm` and `make` respectively.
* On MacOS, these can both be installed using Homebrew, as well as `zip`. Apple clang won't work, as you need a mips target for building the mod code.
* On Linux, install the above packages, and some additional ones: `clang`, `make`, `llvm`, `lld`, `zip`, using your distro's package manager.

You'll also need to build [N64Recomp](https://github.com/N64Recomp/N64Recomp) for the `RecompModTool` utility, and copy it into your clone of rando (or put it on your PATH).

## Building
* First, make sure you've cloned the repo recursively `git clone https://github.com/RecompRando/MMRecompRando --recurse-submodules` or you can update the submodules afterwards `git submodule update --init --recursive`.
* Then, run `make` (with an optional job count) to build the mod code itself.
