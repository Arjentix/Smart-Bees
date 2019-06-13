import pymysql
from pymysql.cursors import DictCursor

class DbHandler:
	def __init__(self):
		# Connection to Db
		self.connection = pymysql.connect(
			host='localhost',
			user='ServiceAdmin',
			password='secret',
			db='SmartHomeService',
			charset='utf8mb4',
			cursorclass=DictCursor
		)

	def __del__(self):
		# Closing connection to Db
		self.connection.close()

	def bind_gate_uid(self, gate, uid):
		# Pushing new data to Db
		with self.connection.cursor() as cursor:
			data = (gate, uid)
			query = 'INSERT INTO gate_uid(gate, uid) VALUES (%s, %s)'
			cursor.execute(query, data)
			self.connection.commit()

			return True

	def get_gate_from_db(self, user_id):
		with self.connection.cursor() as cursor:
			query = 'SELECT gate FROM gate_uid WHERE uid = \'{}\''.format(user_id)
			cursor.execute(query)
			res = cursor.fetchone()
			if res is not None:
				return res['gate']
			else:
				return ''
