#!/usr/bin/env python

from tmdb_api import Configuration, searchMovie, searchPerson, Person, \
                     Movie, Collection, __version__
from request import set_key, set_cache
from locales import Locale, set_locale
from tmdb_exceptions import *

