# The Incrementors

import pandas as pd
import folium
import os

clemson = os.path.join('/home/bsrevill/Documents/gpsdwifi/', 'clemson.json')

wifi_coverage = os.path.join('/home/bsrevill/Documents/gpsdwifi/', 'averages.csv')
wifi_data = pd.read_csv(wifi_coverage)

m = folium.Map(location=[34.6766, -82.8364], zoom_start=17)

folium.Choropleth(
    geo_data=clemson,
    name='choropleth',
    data=wifi_data,
    columns=['ID', 'Signal'],
    key_on='feature.id',
    fill_color='OrRd',
    fill_opacity=0.7,
    line_opacity=0.2,
    legend_name='Signal Strength (dBm)'
).add_to(m)

m.save('wificoverage.html')