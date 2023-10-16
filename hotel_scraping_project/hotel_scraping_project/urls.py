from django.urls import path
from hotels.views import hotel_search


from hotels import views as hotelView

urlpatterns = [
    path('hotel_search', hotelView.hotel_search, name='hotel_search'),
]