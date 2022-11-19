import unreal

def rename_assets(search_string, replace_string, use_case):
    # instances of unreal classes
    system_lib = unreal.SystemLibrary()
    editor_util = unreal.EditorUtilityLibrary()
    string_lib = unreal.StringLibrary()


    # get the selected assets
    selected_assets = editor_util.get_selected_assets()
    num_assets = len(selected_assets)
    replaced = 0

    unreal.log("Selected {} assets".format(num_assets))

    # loop through the selected assets
    for asset in selected_assets:
        asset_name = system_lib.get_object_name(asset)

        unreal.log(asset_name)
        # check if the asset name contains the search string
        if string_lib.contains(asset_name, search_string, use_case=use_case):
            # replace the search string with the replace string
            search_case = unreal.SearchCase.CASE_SENSITIVE if use_case else unreal.SearchCase.IgnoreCase
            replaced_name = string_lib.replace(asset_name, search_string, replace_string, search_case=search_case)
            # rename the asset
            system_lib.rename_object(asset, replaced_name)
            
            replaced += 1
            unreal.log("Replaced {} with {}".format(asset_name, replaced_name))
        
        else:
            unreal.log("{} does not contain the search string".format(asset_name))

    unreal.log("Replaced {} of {} assets".format(replaced, num_assets))

rename_assets()