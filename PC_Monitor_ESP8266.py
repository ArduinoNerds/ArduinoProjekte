import requests
import psutil
import platform
import time

esp_ip = "http://EURE IP ADRESSE VOM MIKROCONTROLLER"  # IP deines ESP8266 anpassen!

while True:
    # CPU-Daten
    cpu_usage = psutil.cpu_percent()
    cpu_freq = psutil.cpu_freq().current if hasattr(psutil, "cpu_freq") else "N/A"
    cpu_cores = psutil.cpu_count(logical=False)  # Physische Kerne
    cpu_name = platform.processor()

    # RAM-Daten
    ram_total = round(psutil.virtual_memory().total / (1024 ** 3), 2)  # GB
    ram_usage = psutil.virtual_memory().percent

    # Festplatten-Daten
    ssd_usage = psutil.disk_usage('/').percent  # Hauptfestplatte (% belegt)

    # Netzwerk-Daten
    net_sent = round(psutil.net_io_counters().bytes_sent / (1024 * 1024), 2)  # MB gesendet
    net_recv = round(psutil.net_io_counters().bytes_recv / (1024 * 1024), 2)  # MB empfangen

    # System-Daten
    os_name = platform.system() + " " + platform.release()
    uptime = round((time.time() - psutil.boot_time()) / 60, 2)  # Minuten seit PC-Start

    data = {
        "cpu_usage": cpu_usage,
        "cpu_freq": round(cpu_freq, 2),
        "cpu_cores": cpu_cores,
        "cpu_name": cpu_name,
        "ram_total": ram_total,
        "ram_usage": ram_usage,
        "ssd_usage": ssd_usage,
        "net_sent": net_sent,
        "net_recv": net_recv,
        "os_name": os_name,
        "uptime": uptime
    }

    try:
        response = requests.get(f"{esp_ip}/update", params=data)
        print(f"Gesendet: {data} - Antwort: {response.text}")
    except:
        print("ESP nicht erreichbar!")

    time.sleep(5)  # Alle 5 Sekunden senden
