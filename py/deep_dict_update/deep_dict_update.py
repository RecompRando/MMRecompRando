from typing import Dict, Any, Union, List
import collections.abc

def deep_dict_update(orig_dict: Dict[str, Any], new_dict: Dict[str, Any]) -> Dict[str, Any]:
    """
    Recursively updates a nested dictionary with the content of another dictionary.

    Parameters:
    - orig_dict (Dict[str, Any]): The original dictionary to be updated.
    - new_dict (Dict[str, Any]): The dictionary containing updates.

    Returns:
    - Dict[str, Any]: The updated dictionary.

    Notes:
    - If a key in `new_dict` is not present in `orig_dict`, it will be added to `orig_dict`.
    - If a value in `new_dict` is a dictionary, the corresponding value in `orig_dict` will be updated recursively.
    - If a value in `new_dict` is a list of dictionaries, each dictionary in the list will be updated recursively.
    - For non-dictionary and non-list values, the value in `orig_dict` will be updated directly.
    """

    orig_dict = dict(orig_dict)
    for key, val in dict(new_dict).items():
        if key not in orig_dict:
            # If key is not present in orig_dict, initialize with an empty dictionary
            orig_dict[key] = {}

        if isinstance(val, collections.abc.Mapping):
            # If both orig_dict[key] and val are dictionaries, recursively update
            tmp = deep_dict_update(orig_dict[key], val)
            orig_dict[key] = tmp
        elif isinstance(val, list):
            # If the value is a list, iterate through the items
            # and apply dict_update for each dictionary in the list
            orig_dict[key] = [
                deep_dict_update(orig_dict[key][i] if i < len(orig_dict[key]) else {}, item) if isinstance(item, collections.abc.Mapping) else item
                for i, item in enumerate(val)
            ]
        else:
            # For non-dictionary and non-list values, update directly
            orig_dict[key] = val

    return orig_dict
