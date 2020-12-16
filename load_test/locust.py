import time
from locust import HttpUser, task, between


class WebsiteUser(HttpUser):
	wait_time = between(1, 5)

	@task
	def inxed_page(self):
		self.client.get(url="/index.html")

	@task
	def slow_page(self):
		self.client.get(url="/cpu.html")
