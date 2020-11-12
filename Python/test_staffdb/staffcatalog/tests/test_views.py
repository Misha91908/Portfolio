import datetime

from django.test import TestCase

# Create your tests here.

from staffcatalog.models import Person
from django.urls import reverse


class PersonListViewTest(TestCase):

    @classmethod
    def setUpTestData(cls):
        # Create 13 persons for pagination tests
        number_of_persons = 13
        for i in range(number_of_persons):
            Person.objects.create(id=i, first_name='Christian %s' % i, last_name='Surname %s' % i,
                                  birth_date=datetime.datetime.now(), email='test@test.org', phone_number='5476568',
                                  job_assign=datetime.datetime.now(), position='Team Lead', department='1 test')

    def test_view_url_exists_at_desired_location(self):
        resp = self.client.get('/staffcatalog/list/')
        self.assertEqual(resp.status_code, 200)

    def test_view_url_accessible_by_name(self):
        resp = self.client.get(reverse('list'))
        self.assertEqual(resp.status_code, 200)

    def test_view_uses_correct_template(self):
        resp = self.client.get(reverse('list'))
        self.assertEqual(resp.status_code, 200)

        self.assertTemplateUsed(resp, 'staffcatalog/person_list.html')

    def test_pagination_is_ten(self):
        resp = self.client.get(reverse('list'))
        self.assertEqual(resp.status_code, 200)
        self.assertTrue('is_paginated' in resp.context)
        self.assertTrue(resp.context['is_paginated'] is True)
        self.assertTrue(len(resp.context['person_list']) == 10)

    def test_lists_all_authors(self):
        # Get second page and confirm it has (exactly) remaining 3 items
        resp = self.client.get(reverse('list') + '?page=2')
        self.assertEqual(resp.status_code, 200)
        self.assertTrue('is_paginated' in resp.context)
        self.assertTrue(resp.context['is_paginated'] == True)
        self.assertTrue(len(resp.context['person_list']) == 3)
