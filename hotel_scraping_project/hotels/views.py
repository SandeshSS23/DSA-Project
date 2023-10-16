from django.shortcuts import render
from .models import Hotel
import requests
from bs4 import BeautifulSoup

def hotel_search(request):
    if request.method == 'POST':
        city = request.POST.get('city')
        url = f'    '
        response = requests.get(url)
        soup = BeautifulSoup(response.text, 'html.parser')

        hotels = []
        hotel_elements = soup.select('.sr_item')
        for element in hotel_elements:
            name = element.select_one('.sr-hotel__name').text.strip()
            price = element.select_one('.bui-price-display__value').text.strip()

            hotel = Hotel(name=name, price=price)
            hotels.append(hotel)
        
        context = {'hotels': hotels, 'city': city}
        return render(request, 'hotels/hotel_list.html', context)
    else:
        return render(request, 'hotels/hotel_search.html')
