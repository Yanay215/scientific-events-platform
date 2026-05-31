import json
import sys
import os

def main():
    if len(sys.argv) < 2:
        print(json.dumps([]))
        return
    query = " ".join(sys.argv[1:]).strip().lower()
    if not query:
        print(json.dumps([]))
        return
    try:
        script_dir = os.path.dirname(os.path.abspath(__file__))
        json_path = os.path.join(script_dir, "russian_universities_ru.json")
        with open(json_path, "r", encoding="utf-8") as f:
            universities = json.load(f)
    except Exception as e:
        print(json.dumps([]))
        return
    results = []
    for university in universities:
        name_ru = university.get("name_ru", "").lower()
        name_en = university.get("name_en", "").lower()
        synonyms =[s.lower() for s in university.get("synonyms", [])]
        if query in name_ru or query in name_en or any(query in s for s in synonyms):
            results.append({
                "name": university.get("name_ru"),
                "synonyms": university.get("synonyms", [])
            })
            if len(results) == 10:
                break
    print(json.dumps(results, ensure_ascii=False))

if __name__ == "__main__":
    main()