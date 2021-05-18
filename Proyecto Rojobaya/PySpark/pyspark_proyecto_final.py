import findspark
findspark.init()
findspark.find()
import pyspark
findspark.find()
from pyspark import SparkContext, SparkConf
from pyspark.sql import SparkSession
conf = pyspark.SparkConf().setAppName('SparkApp').setMaster('local')
from pyspark.sql import SparkSession, functions as F
from urllib.request import urlopen
sc = SparkContext.getOrCreate()
spark = SparkSession.builder.getOrCreate()
spark = SparkSession(sc)
import requests

url = 'https://api.dinamita.site/obtenerData'
myobj = {'fecha1':'2021-05-18 00:00:00','fecha2':'2021-05-19 00:00:00'}
#myobj ={}
jsonData = requests.post(url, json = myobj)
rddData = sc.parallelize([jsonData.text])
resultDF = spark.read.json(rddData)
if "concentracion_gas" in resultDF.columns:
    gas = resultDF.where(resultDF["concentracion_gas"].isNotNull())
    suma_gas = gas.agg(F.sum("concentracion_gas")).collect()[0][0]
    promedio_gas = suma_gas/gas.count()
    print("Promedio Gas: ",promedio_gas)
else:
    print("Promedio Gas: sin registros")
if "temperatura" in resultDF.columns:
    temp = resultDF.where(resultDF["temperatura"].isNotNull())
    suma_temp = temp.agg(F.sum("temperatura")).collect()[0][0]
    promedio_temp = suma_temp/temp.count()
    print("Promedio temperatura: ",promedio_temp)
    hum = resultDF.where(resultDF["humedad"].isNotNull())
    suma_hum = hum.agg(F.sum("humedad")).collect()[0][0]
    promedio_hum = suma_hum/hum.count()
    print("Promedio humedad: ",promedio_hum)
else:
    print("Promedio humedad: sin registros")
if "pulsaciones" in resultDF.columns:
    puls1 = resultDF.where(resultDF["pulsaciones"] == 1)
    puls0 = resultDF.where(resultDF["pulsaciones"] == 0)
    prom_puls1 = (puls1.count()*100)/(puls1.count()+puls0.count())
    print("Promedio de 1's en infrarrojo: ",prom_puls1,"%")
    prom_puls0 = (puls0.count()*100)/(puls1.count()+puls0.count())
    print("Promedio de 0's en infrarrojo: ",prom_puls0,"%")
else:
    print("Promedio pulsaciones: sin registros")
if "movimiento" in resultDF.columns:
    mov1 = resultDF.where(resultDF["movimiento"] == 1)
    mov0 = resultDF.where(resultDF["movimiento"] == 0)
    prom_mov1 = (mov1.count()*100)/(mov1.count()+mov0.count())
    prom_mov0 = (mov0.count()*100)/(mov1.count()+mov0.count())
    print("Promedio de 1's en movimiento: ",prom_mov1,"%")
    print("Promedio de 0's en movimiento: ",prom_mov0,"%")
else:
    print("Promedio movimiento: sin registros")