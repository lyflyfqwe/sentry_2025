from setuptools import find_packages
from setuptools import setup

setup(
    name='rm_decision_system',
    version='0.0.0',
    packages=find_packages(
        include=('rm_decision_system', 'rm_decision_system.*')),
)
