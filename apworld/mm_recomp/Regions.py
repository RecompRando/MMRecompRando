from typing import NamedTuple, Callable, List, Dict
from BaseClasses import CollectionState


class MMRRegionData(NamedTuple):
    connecting_regions: List[str] = []


region_data_table: Dict[str, MMRRegionData] = {
    "Menu": MMRRegionData(["Clock Town"]),
    "Clock Town": MMRRegionData(["The Moon", "Trading Post", "Bomber's Hideout", "Termina Field"]),
    "The Moon": MMRRegionData([]),
    "Trading Post": MMRRegionData([]),
    "Bomber's Hideout": MMRRegionData(["Astral Observatory"]),
    "Astral Observatory": MMRRegionData(["Astral Observatory (Outside)"]),
    "Astral Observatory (Outside)": MMRRegionData(["Termina Field"]),
    "Termina Field": MMRRegionData(["Road to Southern Swamp", "Milk Road", "Path to Mountain Village", "Great Bay", "Road to Ikana"]),
    "Road to Southern Swamp": MMRRegionData(["Termina Field", "Southern Swamp"]),
    "Southern Swamp": MMRRegionData(["Road to Southern Swamp", "Southern Swamp (Deku Palace)"]),
    "Southern Swamp (Deku Palace)": MMRRegionData(["Swamp Spider House", "Deku Palace", "Woodfall"]),
    "Swamp Spider House": MMRRegionData([]),
    "Deku Palace": MMRRegionData([]),
    "Woodfall": MMRRegionData(["Woodfall Temple", "Southern Swamp (Deku Palace)"]),
    "Woodfall Temple": MMRRegionData(["Odolwa's Lair"]),
    "Odolwa's Lair": MMRRegionData([]),
    "Milk Road": MMRRegionData(["Gorman Brothers Track", "Romani Ranch"]),
    "Gorman Brothers Track": MMRRegionData(["Inside Gorman Brothers Track"]),
    "Inside Gorman Brothers Track": MMRRegionData([]),
    "Romani Ranch": MMRRegionData([]),
    "Path to Mountain Village": MMRRegionData(["Mountain Village", "Termina Field"]),
    "Mountain Village": MMRRegionData(["Twin Islands", "Path to Snowhead", "Path to Mountain Village", "Termina Field"]),
    "Twin Islands": MMRRegionData(["Goron Village", "Goron Racetrack"]),
    "Goron Racetrack": MMRRegionData(["Twin Islands"]),
    "Goron Village": MMRRegionData(["Goron Shrine"]),
    "Goron Shrine": MMRRegionData([]),
    "Path to Snowhead": MMRRegionData(["Snowhead","Mountain Village"]),
    "Snowhead": MMRRegionData(["Snowhead Temple", "Path to Snowhead"]),
    "Snowhead Temple": MMRRegionData(["Goht's Lair"]),
    "Goht's Lair": MMRRegionData([]),
    "Great Bay": MMRRegionData(["Ocean Spider House", "Fisherman's House", "Pirates' Fortress", "Pinnacle Rock", "Zora Cape", "Termina Field"]),
    "Ocean Spider House": MMRRegionData([]),
    "Fisherman's House": MMRRegionData([]),
    "Pirates' Fortress": MMRRegionData(["Pirates' Fortress Sewers", "Pirates' Fortress (Interior)"]),
    "Pirates' Fortress Sewers": MMRRegionData(["Pirates' Fortress (Interior)"]),
    "Pirates' Fortress (Interior)": MMRRegionData([]),
    "Pinnacle Rock": MMRRegionData([]),
    "Zora Cape": MMRRegionData(["Zora Hall", "Great Bay Temple", "Great Bay"]),
    "Zora Hall": MMRRegionData([]),
    "Great Bay Temple": MMRRegionData(["Gyorg's Lair"]),
    "Gyorg's Lair": MMRRegionData([]),
    "Road to Ikana": MMRRegionData(["Ikana Graveyard", "Lower Ikana Canyon", "Termina Field"]),
    "Ikana Graveyard": MMRRegionData(["Road to Ikana"]),
    "Lower Ikana Canyon": MMRRegionData(["Secret Shrine", "Upper Ikana Canyon", "Road to Ikana"]),
    "Upper Ikana Canyon": MMRRegionData(["Beneath the Well", "Ikana Castle", "Stone Tower", "Lower Ikana Canyon"]),
    "Secret Shrine": MMRRegionData(["Lower Ikana Canyon"]),
    "Beneath the Well": MMRRegionData(["Ikana Castle", "Upper Ikana Canyon"]),
    "Ikana Castle": MMRRegionData(["Beneath the Well", "Inside Ikana Castle"]),
    "Inside Ikana Castle": MMRRegionData([]),
    "Stone Tower": MMRRegionData(["Stone Tower Temple", "Stone Tower (Inverted)", "Upper Ikana Canyon"]),
    "Stone Tower Temple": MMRRegionData([]),
    "Stone Tower (Inverted)": MMRRegionData(["Stone Tower Temple (Inverted)"]),
    "Stone Tower Temple (Inverted)": MMRRegionData(["Twinmold's Lair"]),
    "Twinmold's Lair": MMRRegionData([]),
}

def get_exit(region, exit_name):
    for exit in region.exits:
        if exit.connected_region.name == exit_name:
            return exit
