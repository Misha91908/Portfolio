# Generated by Django 3.1.3 on 2020-11-07 14:01

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('staffcatalog', '0003_auto_20201107_1358'),
    ]

    operations = [
        migrations.AlterField(
            model_name='person',
            name='job_end',
            field=models.DateField(blank=True, null=True),
        ),
        migrations.AlterField(
            model_name='person',
            name='patronymic',
            field=models.CharField(blank=True, max_length=30),
        ),
    ]
