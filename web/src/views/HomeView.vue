<script lang="ts" setup>
import { NH1, NH2, NP, NA, NUl, NLi, NDataTable, useMessage, NLog } from 'naive-ui'

import { onMounted, ref, watch, h } from 'vue'
import { useRouter } from 'vue-router'

import { TableColumns } from 'naive-ui/es/data-table/src/interface';

const router = useRouter()
const message = useMessage()

console.log(router.currentRoute.value.params.file);


const urlRef = ref<string>("")

const dataRef = ref<Array<any>>([])

const columns: TableColumns<any> = [
	{
		type: 'expand',
		renderExpand: (row: any) => {
			let info = "";
			const history = row.history;
			for (const i in history) {
				const e = history[i];
				console.log(e.rank, e.rank === 1);
				const perf =  e.rank === 1 ? "+INF" : String(e.perf)
				info += `${e.oldRating} -> ${e.newRating}，表现分 ${perf}，排名 ${e.rank}，${e.contestName}。\n`
			}
			return h(NLog, { log: `logs:\n${info}` })
		}
	},
	{ title: "姓名", key: "handle", sorter: 'default' },
	// { title: "班级", key: "organization"},
	{
		title: "Rating",
		key: "rating",
		sorter: {
			compare: (a: any, b: any) => a.rating - b.rating,
			multiple: 1
		},
		defaultSortOrder: 'descend'
	},
	{
		title: "Max Rating",
		key: "maxRating",
		sorter: {
			compare: (a: any, b: any) => a.maxRating - b.maxRating,
			multiple: 2
		}
	},
	{
		title: "参赛场数",
		key: "num",
		sorter: {
			compare: (a: any, b: any) => a.num - b.num,
			multiple: 3
		}
	},
]

watch(() => router.currentRoute.value.params, (toParams, _) => {
	console.log(toParams);

	urlRef.value = String(toParams.file)
	loadData(urlRef.value)
})

const loadData = (url: string) => {
	if (url === 'error')
		return
	url = window.location.origin + window.location.pathname + `/${url}.json`
	fetch(url)
		.then(res => res.json())
		.then(data => {
			let array: Array<any> = []
			for (let d in data) {
				let x = data[d]
				x['num'] = x['history'].length
				array.push(data[d])
			}
			array = array.sort((a, b) => b.rating - a.rating)
			console.log(array)
			dataRef.value = array
		})
		.catch(e => {
			console.error(e)
			message.error('加载失败')
		})
}

onMounted(() => {
	urlRef.value = router.currentRoute.value.params.file as string
	loadData(urlRef.value)
})

</script>

<template>
	<div id="home-container">
		<n-h1> ZAFU ACM 积分榜 </n-h1>
		<n-p> 积分使用 <n-a href="https://en.wikipedia.org/wiki/Elo_rating_system">Elo Rating System</n-a>，仿照 CodeForces 的
			Rating 计算方法。详细算法请访问 <n-a href="#"> 项目主页 </n-a>。 </n-p>

		<n-h2> 赛季 </n-h2>
		<n-p> 我们每个学年会重新计分。 </n-p>
		<n-p>
			<n-ul>
				<n-li>
					<n-a @click="router.push('/list/2022-all')">2022 赛季</n-a>。
				</n-li>
				<n-li>
					<n-a @click="router.push('/list/2021-all')">2021 赛季</n-a>。
				</n-li>
			</n-ul>
		</n-p>
		<div v-if="urlRef !== 'error'">
			<n-h2> Rating </n-h2>
			<n-data-table :columns="columns" :data="dataRef" :row-key="(row: any) => row.handle" />
		</div>
	</div>
</template>

<style scoped>
#home-container {
	max-width: 800px;
	margin: 0 auto;
	padding: 40px;
}
</style>
