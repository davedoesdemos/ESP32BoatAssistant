import json
import urllib.request

# URL for the official modern CANboat v2 database definition file
CANBOAT_JSON_URL = "https://raw.githubusercontent.com/canboat/canboat/refs/heads/master/docs/canboat.json"

def fetch_and_export_pgns():
    print("/* Fetching CANboat definitions... */")
    try:
        # Requesting data from the master branch repository
        req = urllib.request.Request(
            CANBOAT_JSON_URL, 
            headers={'User-Agent': 'Mozilla/5.0'}
        )
        with urllib.request.urlopen(req) as response:
            db = json.loads(response.read().decode('utf-8'))
    except Exception as e:
        print(f"// Error downloading schema: {e}")
        return

    pgns = db.get("PGNs", [])
    
    # Categories mapped by ranges or key patterns
    categories = {
        "System / Network Management PGNs": [],
        "Navigation & GPS PGNs": [],
        "Vessel Environment & Performance PGNs": [],
        "Propulsion & Electrical Systems PGNs": [],
        "Other / Miscellaneous PGNs": []
    }

    # Track seen PGNs to remove duplicate entries with specific variant matches
    seen_pgns = set()

    for p in pgns:
        pgn_num = p.get("PGN")
        if pgn_num in seen_pgns or p.get("Fallback", False):
            continue
        
        seen_pgns.add(pgn_num)
        
        # Clean up labels for C strings (escaping quotes)
        label = p.get("Description", "Unknown").replace('"', '\\"')
        
        # CANboat explicitly flags multi-frame types as "Fast"
        is_fast = "true" if p.get("Type") == "Fast" else "false"
        
        c_entry = f"    {{ {pgn_num:<6}, \"{label}\", {is_fast} }},"

        # Direct sorting logic into logical schema groups
        if pgn_num < 61184 or (126208 <= pgn_num <= 126998):
            categories["System / Network Management PGNs"].append(c_entry)
        elif 129025 <= pgn_num <= 129291 or pgn_num == 127250:
            categories["Navigation & GPS PGNs"].append(c_entry)
        elif 128259 <= pgn_num <= 128275 or 130306 <= pgn_num <= 130585:
            categories["Vessel Environment & Performance PGNs"].append(c_entry)
        elif 127488 <= pgn_num <= 127514:
            categories["Propulsion & Electrical Systems PGNs"].append(c_entry)
        else:
            categories["Other / Miscellaneous PGNs"].append(c_entry)

    # Print the resulting C code structure block
    print("\ntypedef struct {")
    print("    uint32_t pgn;")
    print("    const char *label;")
    print("    bool is_fast_packet;")
    print("} n2k_pgn_meta_t;\n")
    print("static const n2k_pgn_meta_t pgn_directory[] = {")
    
    for cat_name, entries in categories.items():
        if entries:
            print(f"    // --- {cat_name} ---")
            for entry in entries:
                print(entry)
                
    print("};")

if __name__ == "__main__":
    fetch_and_export_pgns()
