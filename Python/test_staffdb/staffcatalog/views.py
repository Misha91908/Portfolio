import statistics
import numpy

from django.http import Http404, HttpResponseRedirect
from django.shortcuts import render
from django import template
from django.db.models.query import QuerySet

# Create your views here.
from django.urls import reverse

from .models import Person, Alphabet
from django.views import generic

register = template.Library()


class PersonListView(generic.ListView):
    model = Person
    paginate_by = 40

    def get_context_data(self, **kwargs):
        context = super(PersonListView, self).get_context_data(**kwargs)
        all_departments = list(dict.fromkeys(Person.objects.values_list('department', flat=True)))
        all_departments.sort()
        context['department_list'] = all_departments

        return context

    def post(self, request):

        state = ''
        departments = ''
        if request.method == 'POST':
            state = str(request.POST.getlist('state[]')).replace('[', '').replace(']', '')
            departments = str(request.POST.getlist('departments[]')).replace('[', '').replace(']', '')

        if state == '':
            state = ' '
        if departments == '':
            departments = ' '

        url = reverse('list_search', kwargs={'state': state, 'departments': departments})

        return HttpResponseRedirect(url)


def search_person(request, state, departments):

    states = []
    departments_list = []
    if state != ' ':
        states = state.split(',')
        for i in range(len(states)):
            states[i] = int(states[i].replace("'", "").replace(" ", ""))
    if departments != ' ':
        departments_list = departments.split(',')
        for i in range(len(departments_list)):
            departments_list[i] = departments_list[i].replace("'", "").lstrip()

    persons = []

    if len(states) != 0:
        if (len(list(numpy.where(numpy.array(states) == 1))[0]) != 0) or \
                (len(list(numpy.where(numpy.array(states) == 2))[0]) != 0 and
                 len(list(numpy.where(numpy.array(states) == 3))[0]) != 0):
            persons = Person.objects.all()
        elif len(list(numpy.where(numpy.array(states) == 2))[0]) != 0:
            persons = Person.objects.filter(job_end__isnull=True)
        elif len(list(numpy.where(numpy.array(states) == 3))[0]) != 0:
            persons = Person.objects.filter(job_end__isnull=False)

    all_departments = list(dict.fromkeys(Person.objects.values_list('department', flat=True)))
    all_departments.sort()

    if len(departments_list) != 0:
        excluded_departments = []

        for i in range(len(all_departments)):
            if len(list(numpy.where(numpy.array(departments_list) == all_departments[i]))[0]) == 0:
                excluded_departments.append(all_departments[i])

        if len(states) == 0:
            persons = Person.objects.all()

        if isinstance(persons, QuerySet):

            for departments in excluded_departments:
                persons = persons.exclude(department=departments)

    return render(request, 'staffcatalog/person_list.html', {'department_list': all_departments, 'person_list': persons})


class PersonDetailView(generic.DetailView):
    model = Person


class AlphabetListView(generic.ListView):
    model = Alphabet
    paginate_by = 40

    def get_queryset(self):
        Alphabet.objects.all().delete()

        all_last_names = list(dict.fromkeys(Person.objects.values_list('last_name', flat=True)))
        all_last_names.sort()

        for i in range(len(all_last_names)):
            Alphabet.objects.create(id=i, last_name=all_last_names[i])

        return Alphabet.objects.all()

    def get_context_data(self, **kwargs):
        context = super(AlphabetListView, self).get_context_data(**kwargs)
        return context


class AlphabetDetailView(generic.DetailView):
    model = Alphabet
    slug_field = 'last_name'
    slug_url_kwarg = 'last_name'

    def get_context_data(self, **kwargs):
        context = super(AlphabetDetailView, self).get_context_data(**kwargs)
        context['persons'] = Person.objects.filter(last_name=self.object.last_name)
        return context


def index(request):
    """
    Функция отображения для домашней страницы сайта.
    """
    # Генерация "количеств" некоторых главных объектов
    num_employees = Person.objects.all().count()

    # Отрисовка HTML-шаблона index.html с данными внутри
    # переменной контекста context
    return render(
        request,
        'index.html',
        context={'num_employees': num_employees},
    )
