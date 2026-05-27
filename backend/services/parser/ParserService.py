import requests
import json
from bs4 import BeautifulSoup
import re
from datetime import datetime

all_indexing_systems = {
    "Универсальные и мультидисциплинарные базы": [
        "Scopus",
        "Web of Science Core Collection",
        "ERIH PLUS",
        "РИНЦ (Российский индекс научного цитирования)",
        "Ядро РИНЦ",
        "RSCI (Russian Science Citation Index)"
    ],
    "Государственные реестры и перечни": [
        "Перечень ВАК (Высшей аттестационной комиссии)",
        "Белый список журналов (РЦНИ)"
    ],
    "Медицина, биология и науки о жизни": [
        "MEDLINE",
        "Embase",
        "Biological Abstracts",
        "Zoological Record"
    ],
    "Сельское хозяйство и пищевая промышленность": [
        "AGRICOLA",
        "CAB Abstracts",
        "FSTA"
    ],
    "Математика, физика и IT": [
        "DBLP",
        "MathSciNet",
        "zbMath",
        "Math-Net.Ru"
    ],
    "Инженерия, физика и науки о Земле": [
        "Compendex",
        "Inspec",
        "GeoRef",
        "GeoBase",
        "Petroleum Abstracts (TULSA)"
    ],
    "Химия": [
        "CAS Core",
        "Chimica"
    ],
    "Экономика и общественные науки": [
        "EconLit",
        "EconBiz",
        "Sociological Abstracts",
        "Historical Abstracts",
        "Worldwide Political Science Abstracts"
    ],
    "Открытый доступ и популяризация науки": [
        "КиберЛенинка"
    ],
    "Национальные фонды и архивы": [
        "НЭБ (Национальная электронная библиотека)"
    ]
}

HEADERS = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36'
}

def get_konferencii_ru_data():
    base_url = "https://konferencii.ru/list"
    today = datetime.now()
    params = {
        "search[startDate]": today.strftime("%Y-%m-%d"),
        "search[start][day]": today.day,
        "search[start][month]": today.month,
        "search[start][year]": today.year,
        "submit": "Искать"
    }
    try:
        response = requests.get(base_url, params=params, headers=HEADERS, timeout=10)
        if response.status_code != 200:
            return {'status': 'error', 'site': 'konferencii.ru', 'message': f"Failed to fetch data, status code: {response.status_code}"}
        soup = BeautifulSoup(response.text, 'html.parser')
        events = []
        cards = soup.select('.index_cat_1st')
        for card in cards:
            if card.find(class_="rekplace"):
                continue
            tit_div = card.find(class_="index_cat_tit")
            if not tit_div or not tit_div.find('a'):
                continue
            a_tag = tit_div.find('a')
            title = a_tag.get_text(strip=True)
            link = 'https://konferencii.ru' + a_tag['href']
            txt_div = card.find(class_="index_cat_txt")
            date_div = txt_div.find(class_="left") if txt_div else None
            date_raw = date_div.get_text(encoding='utf-8') if date_div else ''
            date_clean = re.sub(r'\s+', ' ', date_raw).strip()
            loc_p = txt_div.find('p', class_="ross_p")
            location = loc_p.find('b').get_text(strip=True) if loc_p and loc_p.find('b') else ''
            indexation = []
            if loc_p: indexation = [a.get_text(strip=True) for a in loc_p.find_all('a')]
            topics_div = card.find(class_="index_cat_link")
            topics = []
            if topics_div: topics = [a.get_text(strip=True) for a in topics_div.find_all('a', class_="index_cat_cur")]
            org_p = card.find('p', class_="small_p")
            organizers = ''
            if org_p:
                org_text = org_p.get_text(strip=True)
                organizers = re.sub(r'^Организаторы:\s*', '', org_text)
            event_item ={
                'title': title,
                'link': link,
                'date_info': date_clean,
                'location': location,
                'indexation': indexation,
                'topics': topics,
                'organizers': organizers
            }
            events.append(event_item)
        return {'status': 'success', 'site': 'konferencii.ru', 'count': len(events), 'events': events}
    except Exception as e:
        return {'status': 'error', 'site': 'konferencii.ru', 'message': str(e)}
    
def get_konf_ferenc_ru_data():
    base_url = "https://kon-ferenc.ru/"
    this_year = datetime.now().year
    try:
        response = requests.get(base_url + f"konf{this_year}.html", headers=HEADERS, timeout=10)
        print(response.text)
        soup = BeautifulSoup(response.text, 'html.parser')
        events = []
    except Exception as e:
        print(e)
        return []
    return events

def parse_static_site(url):
    try:
        response = requests.get(url, headers=HEADERS, timeout=10)
        print(response.text)
        soup = BeautifulSoup(response.text, 'html.parser')
        events = []
    except Exception as e:
        print(e)
        return []
    return events

if __name__ == "__main__":
    urls = {
        'msu': "https://msu.ru/science/allevents.html",
        'hse': "https://www.hse.ru/science/HSEconf",
    }
    results = {}
    results['konferencii_ru'] = get_konferencii_ru_data()
    results['konf_ferenc_ru'] = get_konf_ferenc_ru_data()
    for url in urls:
        results[url] = parse_static_site(urls[url])