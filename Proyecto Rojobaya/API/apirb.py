from sanic import Sanic
from sanic.response import json
from sanic import response
from pyArango.connection import *
import json
from datetime import datetime

app = Sanic("App Name")

conn = Connection(arangoURL="http://localhost:8529",username="root", password="password")
db = conn["proyecto_rojobaya"]

@app.route("/guardarData", methods=["POST"])
async def echo(request):
	if request.json:
		doc=(request.json)
		ahora = datetime.now().isoformat()
		doc['fecha_hora'] = ahora
		bind = {"doc": doc}
		aql = "INSERT @doc INTO sensores LET newDoc = NEW RETURN newDoc"
		queryResult = db.AQLQuery(aql, rawResults = True, bindVars=bind)
		res = [a for a in queryResult]
		return response.json(res)
	return json({"R":"No fue un json"})
	
@app.route('/obtenerData',methods=['POST'])
async def estados(request):
	if request.json:
		doc=(request.json)
		#print(doc)
		if doc['fecha1']=='' or doc['fecha2']=='':
			aql = 'FOR doc IN sensores RETURN doc'
			queryResult = db.AQLQuery(aql, rawResults = True, batchSize=100)
			res = [a for a in queryResult]
			return response.json(res)
		else:
			bind = {"doc": doc}
			aql = "FOR doc IN sensores FILTER doc.fecha_hora >= '" +doc['fecha1']+"' AND doc.fecha_hora <= '"+doc['fecha2']+"' RETURN doc"
			queryResult = db.AQLQuery(aql, rawResults = True, batchSize=100)
			res = [a for a in queryResult]
			return response.json(res)
	else:
		aql = 'FOR doc IN sensores RETURN doc'
		queryResult = db.AQLQuery(aql, rawResults = True, batchSize=100)
		res = [a for a in queryResult]
		return response.json(res)

if __name__ == "__main__":
	app.run(host="api.dinamita.site", port=8000)
