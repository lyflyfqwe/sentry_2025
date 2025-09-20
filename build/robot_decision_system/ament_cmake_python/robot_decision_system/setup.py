from setuptools import find_packages
from setuptools import setup

setup(
    name='robot_decision_system',
    version='0.0.0',
    packages=find_packages(
        include=('robot_decision_system', 'robot_decision_system.*')),
)
