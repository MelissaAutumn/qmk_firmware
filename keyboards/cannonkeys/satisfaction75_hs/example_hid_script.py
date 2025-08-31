"""
The script I use for sending data to my keyboard. It's ugly, rough and will need some adjustments for your use.
Make sure you install the following pypi packages:
* hid
* psutil
* pyamdgpuinfo

"""
import datetime
import sys
import time
import zoneinfo
from zoneinfo import ZoneInfo

import hid
import psutil
import pyamdgpuinfo

vendor_id     = 0xCA04
product_id    = 0x0011
wanted_gpu_name = 'AMD Radeon RX 7900 XT'

usage_page    = 0xFF60
usage         = 0x61
report_length = 32

def get_raw_hid_interface():
    device_interfaces = hid.enumerate(vendor_id, product_id)
    raw_hid_interfaces = [i for i in device_interfaces if i['usage_page'] == usage_page and i['usage'] == usage]

    if len(raw_hid_interfaces) == 0:
        return None

    interface = hid.Device(path=raw_hid_interfaces[0]['path'])

    print(f"Manufacturer: {interface.manufacturer}")
    print(f"Product: {interface.product}")

    return interface

def send_raw_report(data):
    interface = get_raw_hid_interface()

    if interface is None:
        print("No device found")
        sys.exit(1)

    request_data = [0x00] * (report_length + 1) # First byte is Report ID
    request_data[1:len(data) + 1] = data
    request_report = bytes(request_data)

    #print("Request:")
    #print(request_report)

    try:
        interface.write(request_report)

        response_report = interface.read(report_length, timeout=1000)

        if response_report[0] == 0x2:
            print('Err:')
            print(response_report)

        #print("Response:")
        #print(response_report)
    finally:
        interface.close()

if __name__ == '__main__':
    print('Starting hid report')

    gpu_obj = None

    gpus = pyamdgpuinfo.detect_gpus()
    for gpu_idx in range(0, gpus):
        gpu = pyamdgpuinfo.get_gpu(gpu_idx)
        if gpu.name == wanted_gpu_name:
            gpu_obj = gpu
            break

    if not gpu_obj:
        print(f'Could not find gpu <{wanted_gpu_name}>. Disabling gpu metrics.')

    # Set RTC
    now = datetime.datetime.now(zoneinfo.ZoneInfo('America/Vancouver'))
    timezone_offset = now.utcoffset()
    # Yolo in a timezone offset to the unix time
    unix_time_kinda = now.timestamp() + ((timezone_offset.seconds / 3600) * 60 * 60)
    send_raw_report([0x99, *int(unix_time_kinda).to_bytes(4)])

    while True:
        gpu_val = 0
        if gpu_obj:
            gpu_val = int(gpu_obj.query_load() * 100.0)

        cpu_val = int(psutil.cpu_percent(interval=0, percpu=False))
        ram_val = int(psutil.virtual_memory().percent)

        report = [
            0x3, cpu_val,
            0x5, ram_val
        ]

        if gpu_obj:
            report += [0x4, gpu_val]

        print(f"Sending <{report}>")
        send_raw_report(report)
        time.sleep(0.1)